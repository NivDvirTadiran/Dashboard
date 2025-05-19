import {Component, Input, OnDestroy, OnInit } from '@angular/core';
import { CommonModule} from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../gs-dashboard-widget-manager.service';
import {BaseWidget} from 'gridstack/dist/angular';
import {BehaviorSubject, interval, Observable, Subject, takeUntil, retry, catchError, throwError, shareReplay, map } from 'rxjs';
import {EmisSoapService} from "src/app/services/emis-soap.service";

const SUPER_GROUP_LIST_WIDGET_ID = 'super-group-list-table';


@Component({
  selector: 'super-group-list-widget',
  templateUrl: './super-group-list-widget.component.html',
  styleUrls: ['./super-group-list-widget.component.scss'],
  standalone: true,
  imports: [CommonModule]
})
export class SuperGroupListWidgetComponent extends BaseWidget implements OnInit, OnDestroy {
  @Input() widget!: WidgetConfig;

  widgetSubject: BehaviorSubject<any> = null;
  stopSignal: Subject<void> = null;
  lastUpdate: number;


  displayedColumns: string[] = ['Group ID', 'Name'];

  constructor(public widgetManager: GsDashboardWidgetManagerService,
              private emisSoapService: EmisSoapService) {
    super();

    console.log("constructor GsWidget2Component");

  }

  ngOnInit(): void {
    this.registerGsWidget(this.widget)
      .subscribe(data => {
        console.log(`Widget ${this.widget.id} of type ${SUPER_GROUP_LIST_WIDGET_ID} added and received data/state:`, data);
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
    //const state = config.state;

    if (config && config.state) {
      config.state.loading = true;

      // Update the active widget BehaviorSubject with current state
      this.widgetSubject?.next(config.state);

      // Actual data fetching (replace with real implementation)
      this.fetchWidgetData(config).subscribe({
        next: (data) => {
          //config.title = data.responseInfoHeader.serversInfo;
          config.state.data = data.returnArray;
          config.state.loading = false;
          config.state.error = null;
          config.state.lastUpdated = Date.now();
          this.lastUpdate = data.responseInfoHeader.serversInfo;

          // Notify subscribers about the updated data
          this.widgetSubject?.next(data.returnArray);
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

  public fetchWidgetData(config: WidgetConfig): Observable<any> {
      return this.emisSoapService.getSuperGroupList().pipe(
        map(data => {
          console.log(`Fetched super-group-list-table:`, data);
          // Assuming data structure is { responseInfoHeader: ..., returnArray: [...] }
          // And we want to return the array part for the widget
          return data || [];
        }),
        catchError(err => {
          console.error('Error fetching super-group-list-table data in manager:', err);
          return throwError(err);
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
}
