import {Component, HostBinding, Input, OnDestroy, OnInit } from '@angular/core';
import {BaseWidget} from 'gridstack/dist/angular';
import {BehaviorSubject, interval, Observable, Subject, takeUntil, retry, catchError, throwError, shareReplay, map } from 'rxjs';
import { Chart, ChartOptions } from 'chart.js';

import {SbGridstackComponent} from "../sb-gridstack.component";
import {FrameComponent} from "./frame-component/frame-component.component";
import {StatusFilterComponent} from "./status-filter/status-filter.component";
import {CommonModule, NgIf } from "@angular/common";
import {GsDashboardWidgetManagerService, WidgetConfig} from "../gs-dashboard-widget-manager.service";
import {ChartRequestParams, EmisSoapService} from "../../../app/services/emis-soap.service";

const EXAMPLE_PIE_CHART_WIDGET_ID = 'example-pie-chart-widget';

@Component({
  selector: "example-pie-chart-widget",
  templateUrl: "./example-pie-chart-widget.component.html",
  styleUrls: ["./example-pie-chart-widget.component.scss"],
  standalone: true,
  imports: [FrameComponent, StatusFilterComponent, NgIf, CommonModule]
})
export class ExamplePieChartWidgetComponent extends BaseWidget implements OnInit, OnDestroy {
    @HostBinding("style.display") display = "contents";

    @Input() widget!: WidgetConfig;



    widgetSubject: BehaviorSubject<any> = null;
    stopSignal: Subject<void> = null;
    lastUpdate: number;

    constructor(public widgetManager: GsDashboardWidgetManagerService,
                //public parent: SbGridstackComponent,
                private emisSoapService: EmisSoapService) {
      super();

      console.log("constructor ExamplePieChartWidgetComponent");

    }


    /** Value props */
    @Input() showWidgetPieChart: boolean = true;
    @Input() prop: string = "קריאות שנפתחו לפי ערוצים";
    @Input() showDiv: boolean = true;
    /** Variant props */
    @Input() property1: number = 1;
    @Input() state: string = "Default";


    ngOnInit(): void {
      this.registerGsWidget(this.widget)
        .subscribe(data => {
          console.log(`Widget ${this.widget.id} of type ${EXAMPLE_PIE_CHART_WIDGET_ID} added and received data/state:`, data);
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
          next: (dataBlob: Blob) => {
            //config.title = data.responseInfoHeader.serversInfo;
            config.state.data = dataBlob;
            config.state.loading = false;
            config.state.error = null;
            config.state.lastUpdated = Date.now();
            this.lastUpdate = Date.now();

            // Notify subscribers about the updated data
            this.widgetSubject?.next(config.state);
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

    public fetchWidgetData(config: WidgetConfig): Observable<Blob> {
      const chartParams: ChartRequestParams = config.settings || {};

      return this.emisSoapService.getChart(chartParams).pipe(
        map(data => {
          console.log(`Fetched example-pie-chart-widget:`, data);
          return data;
        }),
        catchError(err => {
          console.error('Error fetching example-pie-chart-widget data in manager:', err);
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
