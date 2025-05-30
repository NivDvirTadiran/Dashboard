import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule, NgIf } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../base-widget/base-widget.component';
import { Observable, throwError } from 'rxjs';
import { catchError, map } from 'rxjs/operators';
import { EmisSoapService, ChartRequestParams } from '../../../app/services/emis-soap.service';
import { FrameComponent } from './frame-component/frame-component.component';
import { StatusFilterComponent } from './status-filter/status-filter.component';

@Component({
  selector: 'example-pie-chart-widget',
  templateUrl: './example-pie-chart-widget.component.html',
  styleUrls: ['./example-pie-chart-widget.component.scss'],
  standalone: true,
  imports: [FrameComponent, StatusFilterComponent, NgIf, CommonModule, GSBaseWidget]
})
export class ExamplePieChartWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  // These inputs are likely for Storybook or specific internal logic, not directly tied to BaseWidget
  // They should be kept if still used within the component's specific template/logic.
  // For now, assuming they are still needed.
  showWidgetPieChart: boolean = true;
  prop: string = 'קריאות שנפתחו לפי ערוצים';
  showDiv: boolean = true;
  property1: number = 1;
  state: string = 'Default';

  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService,
    public override emisSoapService: EmisSoapService
  ) {
    super(widgetManager);
    console.log('constructor ExamplePieChartWidgetComponent');
  }

  public override fetchData(): void {
    if (this.widget && this.widget.state) {
      this.widget.state.loading = true;
      this.widget.state.error = undefined;

      const chartParams: ChartRequestParams = this.widget.settings || {};

      this.emisSoapService.getChart(chartParams).pipe(
        map(data => {
          console.log(`Fetched example-pie-chart-widget:`, data);
          return data;
        }),
        catchError(err => {
          console.error('Error fetching example-pie-chart-widget data in manager:', err);
          this.widget.state!.error = err.message || 'Failed to fetch chart data';
          return throwError(() => err);
        })
      ).subscribe({
        next: (dataBlob: Blob) => {
          if (this.widget.state) {
            this.widget.state.data = dataBlob;
            this.widget.state.loading = false;
            this.widget.state.lastUpdated = Date.now();
          }
        },
        error: (error) => {
          if (this.widget.state) {
            this.widget.state.loading = false;
            this.widget.state.error = error.message || 'Failed to update widget data';
          }
          console.error(`Widget ${this.widget.id} update failed:`, error);
        }
      });
    }
  }

  override ngOnDestroy(): void {
    super.ngOnDestroy(); // Call base class OnDestroy
  }
}
