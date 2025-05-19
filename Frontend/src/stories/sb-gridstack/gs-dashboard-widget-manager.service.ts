// dashboard-widget-manager.service.ts
import {Injectable} from '@angular/core';
import {BehaviorSubject, interval, Observable, Subject, Subscription, throwError} from 'rxjs';
import {catchError, map, retry, shareReplay, takeUntil} from 'rxjs/operators';
import {TableRow} from 'src/app/dashboard/dashboard.service'; // Adjusted path
import {elementCB, GridstackComponent, NgGridStackWidget, nodesCB} from 'gridstack/dist/angular';
import {GridItemHTMLElement, GridStackWidget} from 'gridstack/dist/types';
import {AgentsListDataItemType, EmisSoapService, SuperGroupListItemType} from '../../app/services/emis-soap.service'; // Corrected path

export interface WidgetConfig {
    id: string; // Unique identifier for the widget
    type: string; // Widget type (e.g., 'table', 'chart', etc.)
    nlat: string; // Position on the dashboard grid
    state?: WidgetState | null; // Optional title for the widget
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
    private widgetConfigs = new Map<string, WidgetConfig>();
    private lastUpdates = new Map<string, number>();



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

    constructor() {
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
        if (this.grid) {
          this.grid.addWidget(widget);
        } else {
            console.error('Grid component reference not set or grid not initialized');
        }
    }

    setWidgetConfig(config: WidgetConfig): void {
        this.widgetConfigs.set(config.id, config);
    }

    getWidgetConfig(widgetId: string): WidgetConfig | undefined {
      return this.widgetConfigs.get(widgetId);
    }

    getWidgetState(widgetId: string): WidgetState | undefined {
    return this.widgetConfigs.get(widgetId).state;
  }

    getRegisteredWidgets(): WidgetConfig[] {
        return Array.from(this.widgetConfigs.values());
    }

    initGsWidget(config: WidgetConfig): WidgetConfig {
        // Initialize widget state
        config.state = {
          id: config.id,
          data: null,
          loading: false,
          error: null,
          lastUpdated: null
        };

        this.setWidgetConfig(config);

        let gridWidget: NgGridStackWidget = {
          autoPosition: true,
          w: 2,
          h: 1,
          selector: config.type,
          input: { widget: config},
          id: String(config.id),
        } as NgGridStackWidget;

        // Add the widget to grid if grid is available
        this.addWidgetToGrid(gridWidget);

        return config;

    }

    removeWidget(widgetId: string): void {
      this.widgetConfigs.delete(widgetId);
    }
}
