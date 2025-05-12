// dashboard-widget-manager.service.ts
import { Injectable } from '@angular/core';
import {BehaviorSubject, Observable, Subject, interval, throwError, Subscription, of} from 'rxjs';
import { catchError, retry, shareReplay, takeUntil, map } from 'rxjs/operators';
import { HttpClient } from '@angular/common/http';
import {TableRow} from 'src/app/dashboard/dashboard.service'; // Adjusted path
import {BaseWidget, GridstackComponent, GridstackModule, NgGridStackOptions, NgGridStackWidget, elementCB, gsCreateNgComponents, nodesCB} from 'gridstack/dist/angular';
import { GridStackWidget } from 'gridstack/dist/types';
import { EmisSoapService, AgentsListDataItemType } from '../../app/services/emis-soap.service'; // Corrected path

export interface WidgetConfig {
    id: string; // Unique identifier for the widget
    type: string; // Widget type (e.g., 'table', 'chart', etc.)
    nlat: string; // Position on the dashboard grid
    dataSource: string; // API endpoint or data source for the widget
    updateInterval: number; // Data refresh interval in milliseconds
    title?: string; // Optional title for the widget
    settings?: Record<string, any>; // Additional settings specific to the widget
    resizable?: boolean; // Whether the widget can be resized
    draggable?: boolean; // Whether the widget can be dragged
};

export interface WidgetState {
    id: string; // Matches the widget ID
    data: any; // Current data for the widget
    loading: boolean; // Whether the widget is loading data
    error?: string; // Error message if data fetching fails
    lastUpdated?: number; // Timestamp of the last successful update
}

@Injectable({
    providedIn: 'root' // Or provide in a specific module if not globally available for stories
})
export class GsDashboardWidgetManagerService {
    private activeWidgets = new Map<string, BehaviorSubject<any>>();
    private widgetConfigs = new Map<string, WidgetConfig>();
    private widgetStates = new Map<string, WidgetState>();
    private gridStackWidget = new Map<string, GridStackWidget>();
    private lastUpdates = new Map<string, number>();
    private unsubscribe = new Map<string, Subject<void>>();



    tableObj: TableRow[] = [];

  public exampleDataNew: string =
  '[{"agentName":"Agent Name","agentExten":"Agent Exten.","state":"State","agentNo":"Agent No.","stateTime":"State Time","releaseCode":"Release Code","aCDCalls":"ACD Calls","nonACDCalls":"Non ACD Calls","dNIS":"DNIS","aN":"ANI"},' +
  '{"agentName":"Agent1","agentExten":"0","state":"Logout","agentNo":"1001","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
  '{"agentName":"Agent2","agentExten":"0","state":"Logout","agentNo":"1002","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
  '{"agentName":"Agent5","agentExten":"0","state":"Logout","agentNo":"1005","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
  '{"agentName":"Agent3","agentExten":"0","state":"Logout","agentNo":"1003","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
  '{"agentName":"Agent6","agentExten":"0","state":"Logout","agentNo":"1006","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"}]';


    public subscription: Subscription;
    private _gridCompRef: GridstackComponent | null = null;

    constructor(private emisSoapService: EmisSoapService) {
        this.tableObj = JSON.parse(this.exampleDataNew);
    }

    // Method to set the gridstack component reference from the parent component
    setGridComponent(gridComp: GridstackComponent): void {
        this._gridCompRef = gridComp;
        console.log('GridStack component reference set in widget manager');
    }

    // Getter to access the gridstack component and its grid
    get grid() {
        return this._gridCompRef?.grid;
    }

    // Method to add widgets to the grid
    addWidgetToGrid(widget: NgGridStackWidget): void {
        if (this._gridCompRef?.grid) {
            this._gridCompRef.grid.addWidget(widget);
        } else {
            console.error('Grid component reference not set or grid not initialized');
        }
    }

    getRegisteredWidgets(): WidgetConfig[] {
        return Array.from(this.widgetConfigs.values());
    }

    registerWidget(config: WidgetConfig): Observable<any> {
        this.widgetConfigs.set(config.id, config);

        // Initialize widget state
        this.widgetStates.set(config.id, {
            id: config.id,
            data: null,
            loading: false,
            error: null,
            lastUpdated: null
        });

        let gridWidget: NgGridStackWidget = {
          autoPosition: true,
          w: 2,
          h: 1,
          selector: 'gs-widget',
          input: { widget: config},
          id: String(config.id),
        } as NgGridStackWidget;

        // Add the widget to grid if grid is available
        if (this._gridCompRef?.grid) {
            this._gridCompRef.grid.addWidget(gridWidget);
        }

        if (!this.activeWidgets.has(config.id)) {
            // Create BehaviorSubject for this widget
            const widgetSubject = new BehaviorSubject<any>(null);
            this.activeWidgets.set(config.id, widgetSubject);

            // Create unsubscribe subject for this widget
            const stopSignal = new Subject<void>();
            this.unsubscribe.set(config.id, stopSignal);

            // Set up initial data fetch
            this.updateWidget(config.id);

            // Set up periodic data fetching
            interval(config.updateInterval)
                .pipe(
                    takeUntil(stopSignal),
                    retry(3),
                    catchError(error => {
                        console.error(`Widget ${config.id} update failed:`, error);
                        return throwError(() => error);
                    }),
                    shareReplay(1)
                )
                .subscribe(() => this.updateWidget(config.id));
        }

        return this.activeWidgets.get(config.id).asObservable();
    }

    private updateWidget(widgetId: string) {
        const config = this.widgetConfigs.get(widgetId);
        const state = this.widgetStates.get(widgetId);

        if (config && state) {
            state.loading = true;

            // Update the active widget BehaviorSubject with current state
            this.activeWidgets.get(widgetId)?.next(state);

            // Actual data fetching (replace with real implementation)
            this.fetchWidgetData(config).subscribe({
                next: (data) => {
                    state.data = data;
                    state.loading = false;
                    state.error = null;
                    state.lastUpdated = Date.now();
                    this.lastUpdates.set(widgetId, state.lastUpdated);

                    // Notify subscribers about the updated data
                    this.activeWidgets.get(widgetId)?.next(data);
                },
                error: (error) => {
                    state.loading = false;
                    state.error = error.message || 'Failed to update widget data';
                    console.error(`Widget ${widgetId} update failed:`, error);

                    // Notify subscribers about the error state
                    this.activeWidgets.get(widgetId)?.next(state.data);
                }
            });
        }
    }

    private fetchWidgetData(config: WidgetConfig): Observable<any> {
        // USE THE NEW EMIS SOAP SERVICE FOR SPECIFIC WIDGET TYPES
        if (config.type === 'agent-list-table') {
            return this.emisSoapService.getAgentsList().pipe(
                map((agents: AgentsListDataItemType[]) => {
                    if (!agents || !Array.isArray(agents) || agents.length === 0) {
                        console.warn('AgentsList response is not a valid array or is empty:', agents);
                        return [];
                    }
                    const headerRow = {agentName: 'Agent Name', agentExten: 'Extension', state: 'State', agentNo: 'Agent No.', stateTime: 'State Time', releaseCode: 'Release Code', aCDCalls: 'ACD Calls', nonACDCalls: 'Non ACD Calls', dNIS: 'DNIS', aN: 'ANI'};
                    const dataRows = agents.map(agent => ({
                        agentName: agent.agentName,
                        agentExten: agent.agentNumber,
                        state: 'N/A',
                        agentNo: String(agent.agentId),
                        stateTime: 'N/A',
                        releaseCode: 'N/A',
                        aCDCalls: 'N/A',
                        nonACDCalls: 'N/A',
                        dNIS: 'N/A',
                        aN: 'N/A'
                        // Map other fields as needed
                    }));
                    return [headerRow, ...dataRows];
                }),
                catchError(error => {
                    console.error(`Error fetching data for ${config.id} of type ${config.type}:`, error);
                    return throwError(() => error);
                })
            );
        }
        if (config.type === 'group-list-table') {
            return this.emisSoapService.getGroupList().pipe(
                map(data => {
                    console.log('Fetched group list data in manager:', data);
                    return data;
                }),
                catchError(err => {
                    console.error('Error fetching group list data in manager:', err);
                    return throwError(err);
                })
            );
        }
        if (config.type === 'group-detail-full-info' && config.settings?.['groupId']) {
            return this.emisSoapService.getGroupDetailFullInfo(config.settings['groupId']).pipe(
                map(data => {
                    console.log(`Fetched group-detail-full-info for group ${config.settings?.['groupId']}:`, data);
                    return data;
                }),
                catchError(err => {
                    console.error(`Error fetching group-detail-full-info for group ${config.settings?.['groupId']}:`, err);
                    return throwError(err);
                })
            );
        }
        if (config.type === 'group-queue-info' && config.settings?.['groupId']) {
            return this.emisSoapService.getGroupQueInfo(config.settings['groupId']).pipe(
                map(data => {
                    console.log(`Fetched group-queue-info for group ${config.settings?.['groupId']}:`, data);
                    return data;
                }),
                catchError(err => {
                    console.error(`Error fetching group-queue-info for group ${config.settings?.['groupId']}:`, err);
                    return throwError(err);
                })
            );
        }
        if (config.type === 'group-agents-info' && config.settings?.['groupId']) {
            return this.emisSoapService.getGroupAgentsInfo(config.settings['groupId']).pipe(
                map(data => {
                    console.log(`Fetched group-agents-info for group ${config.settings?.['groupId']}:`, data);
                    return data;
                }),
                catchError(err => {
                    console.error(`Error fetching group-agents-info for group ${config.settings?.['groupId']}:`, err);
                    return throwError(err);
                })
            );
        }
        if (config.type === 'ivr-application-info' && config.settings?.['ivrId']) {
            return this.emisSoapService.getIvrApplicationInfo(config.settings['ivrId']).pipe(
                map(data => {
                    console.log(`Fetched ivr-application-info for IVR ${config.settings?.['ivrId']}:`, data);
                    return data;
                }),
                catchError(err => {
                    console.error(`Error fetching ivr-application-info for IVR ${config.settings?.['ivrId']}:`, err);
                    return throwError(err);
                })
            );
        }
        if (config.type === 'ivr-port-info' && config.settings?.['portId']) {
            return this.emisSoapService.getIvrPortInfo(config.settings['portId']).pipe(
                map(data => {
                    console.log(`Fetched ivr-port-info for Port ${config.settings?.['portId']}:`, data);
                    return data;
                }),
                catchError(err => {
                    console.error(`Error fetching ivr-port-info for Port ${config.settings?.['portId']}:`, err);
                    return throwError(err);
                })
            );
        }
        // Add more conditions for other SOAP-based widgets
        if (config.type === 'brief-agents-data') {
            return this.emisSoapService.getBriefAgents().pipe(
                map(data => {
                    // Transform data as needed for the specific widget that will display it
                    console.log(`Fetched brief-agents-data:`, data);
                    return data; // Or some processed form of it
                }),
                catchError(error => {
                    console.error(`Error fetching data for ${config.id} of type ${config.type}:`, error);
                    return throwError(() => error);
                })
            );
        }
        if (config.type === 'super-group-list-table') {
            return this.emisSoapService.getSuperGroupList().pipe(
                map(data => {
                    console.log(`Fetched super-group-list-table:`, data);
                    // Assuming data structure is { responseInfoHeader: ..., returnArray: [...] }
                    // And we want to return the array part for the widget
                    return data.returnArray || [];
                }),
                catchError(err => {
                    console.error('Error fetching super-group-list-table data in manager:', err);
                    return throwError(err);
                })
            );
        }
        if (config.type === 'group-abandoned-info' && config.settings?.['groupId']) {
            return this.emisSoapService.getGroupAbandonedInfo(config.settings['groupId']).pipe(
                map(data => {
                    console.log(`Fetched group-abandoned-info for group ${config.settings?.['groupId']}:`, data);
                    return data;
                }),
                catchError(err => {
                    console.error(`Error fetching group-abandoned-info for group ${config.settings?.['groupId']}:`, err);
                    return throwError(err);
                })
            );
        }
        // Add more conditions for other SOAP-based widgets here...

        // Mock implementation for now for other types (if not covered above)
        return new Observable(observer => {
            setTimeout(() => {
                let mockData;

                switch (config.type) {
                    case 'table':
                        mockData =  this.tableObj ;
                        break;
                    case 'pie-chart':
                        mockData = {
                            labels: ['צ׳אט באתר (46%)', 'דוא״ל (20%)', 'Whatsapp (16%)', '(12%) sms', 'שיחה קולית (5%)', 'מסנג׳ר (1%)'],
                            datasets: [{
                                data: [46, 20, 16, 12, 5, 1],
                                backgroundColor: [
                                    'rgba(255, 81, 81, 1)',
                                    'rgba(67, 165, 255, 1)',
                                    'rgba(250, 195, 33, 1)',
                                    'rgba(153, 99, 213, 1)',
                                    'rgba(250, 150, 33, 1)',
                                    'rgba(31, 199, 119, 1)'
                                ],
                                borderColor: [
                                    'rgba(255, 81, 81, 0.2)',
                                    'rgba(67, 165, 255, 0.2)',
                                    'rgba(250, 195, 33, 0.2)',
                                    'rgba(153, 99, 213, 0.2)',
                                    'rgba(250, 150, 33, 0.2)',
                                    'rgba(31, 199, 119, 0.2)'
                                ],
                                borderWidth: 0.1
                            }]
                        };
                        break;
                    case 'doughnut':
                        mockData = {
                            labels: ['A', 'B', 'C', 'D'],
                            datasets: [{
                                data: [
                                    Math.round(Math.random() * 100),
                                    Math.round(Math.random() * 100),
                                    Math.round(Math.random() * 100),
                                    Math.round(Math.random() * 100)
                                ],
                                backgroundColor: ['#FF6384', '#36A2EB', '#FFCE56', '#4BC0C0']
                            }]
                        };
                        break;
                    default:
                        // Fallback for original table type or other non-SOAP widgets
                        if (config.type === 'table') {
                            mockData =  this.tableObj;
                        } else {
                            mockData = { message: 'Sample data for ' + config.type };
                        }
                }

                observer.next(mockData);
                observer.complete();
            }, 500);
        });
    }

    getWidgetState(widgetId: string): WidgetState | undefined {
        return this.widgetStates.get(widgetId);
    }

    getLastUpdateTime(widgetId: string): number | undefined {
        return this.lastUpdates.get(widgetId);
    }

    updateWidgetConfig(widgetId: string, updates: Partial<WidgetConfig>): void {
        const config = this.widgetConfigs.get(widgetId);
        if (config) {
            this.widgetConfigs.set(widgetId, { ...config, ...updates });
        }
    }

  /** called whenever items change size/position/etc.. */
  public onChange(data: nodesCB) {
    // TODO: update our TEMPLATE list to match ?
    // NOTE: no need for dynamic as we can always use grid.save() to get latest layout, or grid.engine.nodes
    console.log('change ', data.nodes.length > 1 ? data.nodes : data.nodes[0]);
  }

  public onResizeStop(data: elementCB) {
    console.log('resizestop ', data.el.gridstackNode);
  }


  refreshWidget(widgetId: string): void {
      // Manually trigger a widget data refresh
      this.updateWidget(widgetId);
  }

  unregisterWidget(widgetId: string): void {
        // Stop ongoing subscriptions
        const stopSignal = this.unsubscribe.get(widgetId);
        if (stopSignal) {
            stopSignal.next();
            stopSignal.complete();
            this.unsubscribe.delete(widgetId);
        }

        // Clean up all data structures
        this.activeWidgets.delete(widgetId);
        this.widgetConfigs.delete(widgetId);
        this.widgetStates.delete(widgetId);
        this.lastUpdates.delete(widgetId);

        this.subscription?.unsubscribe();
    }
}
