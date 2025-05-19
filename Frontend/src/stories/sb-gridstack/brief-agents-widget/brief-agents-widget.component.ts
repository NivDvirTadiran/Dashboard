import {Component, Input, OnDestroy, OnInit, Optional } from '@angular/core';
import { CommonModule} from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../gs-dashboard-widget-manager.service';
import {BaseWidget} from 'gridstack/dist/angular';
import {BehaviorSubject, interval, Observable, Subject, takeUntil, retry, catchError, throwError, shareReplay, map } from 'rxjs';
import {DataItemRow, DataItemType, EmisSoapService, ReturnMatrixDataTypeBG} from "src/app/services/emis-soap.service";


const BRIEF_AGENTS_WIDGET_ID = 'brief-agents-widget';


@Component({
  selector: 'brief-agents-widget',
  templateUrl: './brief-agents-widget.component.html',
  styleUrls: ['./brief-agents-widget.component.scss'],
  standalone: true,
  imports: [CommonModule]
})
export class BriefAgentsWidgetComponent extends BaseWidget implements OnInit, OnDestroy {
  @Input() widget!: WidgetConfig;

  widgetSubject: BehaviorSubject<any> = null;
  stopSignal: Subject<void> = null;
  lastUpdate: number;


  displayedColumns: string[] = ["Agent Name", "Agent Exten.", "State", "Agent No.", "State Time", "Total Release Time", "ACD Calls", "NON-ACD Calls", "DNIS", "ANI"];
  row: string[] = [];
  table: any;


  constructor(public widgetManager: GsDashboardWidgetManagerService,
              private emisSoapService: EmisSoapService) {
    super();

    console.log("constructor BriefAgentsWidgetComponent");

  }

  ngOnInit(): void {
    this.registerGsWidget(this.widget)
      .subscribe(data => {
        console.log(`Widget ${this.widget.id} of type ${BRIEF_AGENTS_WIDGET_ID} added and received data/state:`, data);
      });
  }

  registerGsWidget(config: WidgetConfig): Observable<any> {
    if (!this.widgetSubject) {
      // Create BehaviorSubject for this widget
      this.widgetSubject = new BehaviorSubject<any>(null);

      // Create unsubscribe subject for this widget
      this.stopSignal = new Subject<void>();

      // Set up initial data fetch
      this.updateGsWidget(this.widget.id);

      // Set up periodic data fetching
      interval(config.updateInterval)
        .pipe(
          takeUntil(this.stopSignal),
          retry(3),
          catchError(error => {
            console.error(`Widget ${config.id} update failed:`, error);
            return throwError(() => error);
          }),
          shareReplay(1)
        )
        .subscribe(() => this.updateGsWidget(this.widget.id));
    }

    return this.widgetSubject.asObservable();
  }


  public updateGsWidget(widgetId: string) {
    const config = this.widget;

    if (config && config.state) {
      config.state.loading = true;

      // Update the active widget BehaviorSubject with current state
      this.widgetSubject?.next(config.state);

      // Actual data fetching (replace with real implementation)
      this.fetchWidgetData(config).subscribe({
        next: (data) => {
          config.state.data = data;
          config.state.loading = false;
          config.state.error = null;
          config.state.lastUpdated = Date.now();
          this.lastUpdate = Date.now();

          // Notify subscribers about the updated data
          this.widgetSubject?.next(data);
        },
        error: (error) => {
          config.state.loading = false;
          config.state.error = error.message || 'Failed to update widget data';
          console.error(`Widget ${widgetId} update failed:`, error);

          // Notify subscribers about the error state
          this.widgetSubject?.next(config.state.data);
        }
      });
    }
  }

  public fetchWidgetData(config: WidgetConfig): Observable<ReturnMatrixDataTypeBG> {
    return this.emisSoapService.getBriefAgents().pipe(
      map(data => {
        if (data && data.returnMatrix) {
          data.returnMatrix.forEach(item => {
            if (item.returnArray && item.returnArray.length === 1 && typeof item.returnArray[0]?.value === 'string') {
              const rawDataString = item.returnArray[1].value;
              const parsedValues = this.parseAgentDataString(rawDataString);

              const originalDataItem = item.returnArray[0];
              item.returnArray = parsedValues.map(val => ({
                ...originalDataItem, // Preserve other properties from the original item
                val: val[1]
              }));
            }
          });
        }
        console.log(`Fetched brief-agents-data:`, data);
        return data;
      }),
      catchError(error => {
        console.error(`Error fetching data for ${config.id} of type ${config.type}:`, error);
        return throwError(() => error);
      })
    );
  }

  /**
   * Refresh the data for this widget
   * @param widgetId The ID of the widget to refresh
   */
  refreshWidget(widgetId: string): void {
    // Call the widget manager's refresh method for this specific widget
     this.updateGsWidget(this.widget.id);
  }

  ngOnDestroy(): void {
    this.widgetSubject.unsubscribe();

    // Stop ongoing subscriptions
    if (this.stopSignal) {
      this.stopSignal.next();
      this.stopSignal.complete();
    }

    this.widgetManager.removeWidget(this.widget.id);
  }

  public parseAgentDataString(rawDataString: string): string[] {
    const dataMap = new Map<string, string>();
    const parts = rawDataString.split(',');


    parts.forEach(part => {
      let cell = part.split(';');
      dataMap.set(cell[2], cell[3]);
    })

    const result: string[] = [];
    // Define mappings from displayedColumn names to the keys expected in the rawDataString
    const keyMappings: { [colName: string]: string } = {
      "Agent Name": "Agent Name",
      "Agent Exten.": "Agent Exten.",
      "State": "State",
      "Agent No.": "Agent No", // Key in string is "Agent No"
      "State Time": "State Time",
      "Total Release Time": "Total Release Time", // Updated from "Release Code"
      "ACD Calls": "ACD Calls",
      "NON-ACD Calls": "NON-ACD Calls", // Updated from "Non ACD Calls"
      "DNIS": "DNIS",
      "ANI": "ANI",
    };


    //dataMap.forEach((value, key) => {result.push(value);} )

    for (const colName of this.displayedColumns) {
      const keyInMap = keyMappings[colName] || colName; // Use mapped key or colName itself
      if (dataMap.has(keyInMap)) {
        result.push(dataMap.get(keyInMap)!);
      } else {
         //console.warn(`Key "${keyInMap}" for column "${colName}" not found in parsed dataMap.`);
      }
    }
    return result;
  }
}
