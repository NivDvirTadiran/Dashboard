import {Component, Input, OnDestroy, OnInit } from '@angular/core';
import { CommonModule} from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../gs-dashboard-widget-manager.service';
import {BaseWidget} from 'gridstack/dist/angular';
import {BehaviorSubject, interval, Observable, Subject, takeUntil, retry, catchError, throwError, shareReplay, map } from 'rxjs';
import {EmisSoapService, GroupListDataItemType} from "src/app/services/emis-soap.service";

const GROUP_LIST_WIDGET_ID = 'group-list-widget';


@Component({
  selector: 'group-list-widget',
  templateUrl: './group-list-widget.component.html',
  styleUrls: ['./group-list-widget.component.scss'],
  standalone: true,
  imports: [CommonModule]
})
export class GroupListWidgetComponent extends BaseWidget implements OnInit, OnDestroy {
  @Input() widget!: WidgetConfig;

  widgetSubject: BehaviorSubject<any> = null;
  stopSignal: Subject<void> = null;
  lastUpdate: number;


  displayedColumns: string[] = ['Group ID', 'Name', 'Number', 'Email Enabled'];

  constructor(public widgetManager: GsDashboardWidgetManagerService,
              private emisSoapService: EmisSoapService) {
    super();

    console.log("constructor GroupListWidgetComponent");

  }

  ngOnInit(): void {
    this.registerGsWidget(this.widget)
      .subscribe(data => {
        console.log(`Widget ${this.widget.id} of type ${GROUP_LIST_WIDGET_ID} added and received data/state:`, data);
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

  public fetchWidgetData(config: WidgetConfig): Observable<GroupListDataItemType[]> {
    return this.emisSoapService.getGroupList().pipe(
      map(data => {
        console.log(`Fetched group-list-table:`, data);
        return data || [];
      }),
      catchError(err => {
        console.error('Error fetching group-list-table data in manager:', err);
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
    this.widgetManager.refreshWidget(widgetId);
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
