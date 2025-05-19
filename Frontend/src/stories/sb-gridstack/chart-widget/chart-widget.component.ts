import { Component, Input, OnDestroy, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../gs-dashboard-widget-manager.service';
import { BaseWidget } from 'gridstack/dist/angular';
import { BehaviorSubject, interval, Observable, Subject, throwError } from 'rxjs';
import { catchError, map, retry, shareReplay, takeUntil } from 'rxjs/operators';
import { EmisSoapService, ChartRequestParams } from 'src/app/services/emis-soap.service';
import { DomSanitizer, SafeUrl } from '@angular/platform-browser';

const CHART_WIDGET_ID_PREFIX = 'chart-widget-';

@Component({
  selector: 'chart-widget',
  templateUrl: './chart-widget.component.html',
  styleUrls: ['./chart-widget.component.scss'],
  standalone: true,
  imports: [CommonModule]
})
export class ChartWidgetComponent extends BaseWidget implements OnInit, OnDestroy {
  @Input() widget!: WidgetConfig;

  widgetSubject: BehaviorSubject<any> = null;
  stopSignal: Subject<void> = null;
  lastUpdate: number;
  chartImageUrl: SafeUrl | null = null;

  constructor(
    public widgetManager: GsDashboardWidgetManagerService,
    private emisSoapService: EmisSoapService,
    private sanitizer: DomSanitizer
  ) {
    super();
    console.log("constructor ChartWidgetComponent");
  }

  ngOnInit(): void {
    this.registerGsWidget(this.widget)
      .subscribe(dataOrState => {
        // If dataOrState is a Blob, it's chart data, otherwise it's state
        if (dataOrState instanceof Blob) {
          this.updateChartImage(dataOrState);
        } else if (dataOrState && dataOrState.data instanceof Blob) {
          this.updateChartImage(dataOrState.data);
        }
        console.log(`Widget ${this.widget.id} of type ${CHART_WIDGET_ID_PREFIX} added and received data/state:`, dataOrState);
      });
  }

  registerGsWidget(config: WidgetConfig): Observable<any> {
    if (!this.widgetSubject) {
      this.widgetSubject = new BehaviorSubject<any>(null);
      this.stopSignal = new Subject<void>();

      // Initialize widget state if not already present
      if (!config.state) {
        config.state = {
          id: config.id,
          data: null,
          loading: false,
          error: null,
          lastUpdated: null
        };
      }
      // Ensure widget config is managed if this component is used standalone from gs-dashboard-manager
      this.widgetManager.setWidgetConfig(config);


      this.updateGsWidget(this.widget.id);

      interval(config.updateInterval)
        .pipe(
          takeUntil(this.stopSignal),
          retry(3),
          catchError(error => {
            console.error(`Widget ${config.id} update failed:`, error);
            this.widget.state.loading = false;
            this.widget.state.error = error.message || 'Periodic update failed';
            this.widgetSubject?.next(this.widget.state);
            return throwError(() => error);
          }),
          shareReplay(1)
        )
        .subscribe(() => this.updateGsWidget(this.widget.id));
    }
    return this.widgetSubject.asObservable();
  }

  public updateGsWidget(widgetId: string): void {
    const config = this.widgetManager.getWidgetConfig(widgetId) || this.widget;

    if (config && config.state) {
      config.state.loading = true;
      config.state.error = null; // Clear previous errors
      this.chartImageUrl = null; // Clear previous image
      this.widgetSubject?.next(config.state);

      this.fetchWidgetData(config).subscribe({
        next: (dataBlob: Blob) => {
          this.updateChartImage(dataBlob);
          config.state.data = dataBlob; // Store blob in state if needed, or just use chartImageUrl
          config.state.loading = false;
          config.state.error = null;
          config.state.lastUpdated = Date.now();
          this.lastUpdate = Date.now(); // Or some info from response if available

          this.widgetSubject?.next(config.state); // Notify with updated state
        },
        error: (error) => {
          config.state.loading = false;
          config.state.error = error.message || 'Failed to update widget data';
          this.chartImageUrl = null;
          console.error(`Widget ${widgetId} update failed:`, error);
          this.widgetSubject?.next(config.state); // Notify with error state
        }
      });
    }
  }

  private updateChartImage(blob: Blob): void {
    if (this.chartImageUrl) {
      URL.revokeObjectURL(this.chartImageUrl.toString()); // Revoke old URL to free resources
    }
    const objectURL = URL.createObjectURL(blob);
    this.chartImageUrl = this.sanitizer.bypassSecurityTrustUrl(objectURL);
  }

  public fetchWidgetData(config: WidgetConfig): Observable<Blob> {
    const chartParams: ChartRequestParams = config.settings || {};
    // Ensure essential parameters are present or provide defaults if applicable
    // For example, if 'type' is mandatory:
    // if (!chartParams.type) {
    //   return throwError(() => new Error('Chart type is missing in widget settings'));
    // }

    return this.emisSoapService.getChart(chartParams).pipe(
      map(blob => {
        console.log(`Fetched chart data for widget ${config.id}:`, blob);
        return blob;
      }),
      catchError(err => {
        console.error(`Error fetching chart data for widget ${config.id} in component:`, err);
        return throwError(err);
      })
    );
  }

  refreshWidget(widgetId: string): void {
    this.updateGsWidget(widgetId);
  }

  ngOnDestroy(): void {
    if (this.chartImageUrl) {
      URL.revokeObjectURL(this.chartImageUrl.toString());
    }
    if (this.widgetSubject) {
      this.widgetSubject.complete(); // Use complete for BehaviorSubject
    }
    if (this.stopSignal) {
      this.stopSignal.next();
      this.stopSignal.complete();
    }
    // Optionally remove from widgetManager if it was added here,
    // but typically gs-dashboard-manager handles unregistration.
    // this.widgetManager.removeWidget(this.widget.id);
  }
}
