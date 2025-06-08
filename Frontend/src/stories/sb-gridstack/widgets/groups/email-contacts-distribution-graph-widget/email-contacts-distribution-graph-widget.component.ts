import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule, NgIf } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { Observable, throwError } from 'rxjs';
import { catchError, finalize, map } from 'rxjs/operators';
import {EmisSoapService, ChartRequestParams, ChartDataResponse} from 'src/app/services/emis-soap.service';
import { FrameComponent } from './frame-component/frame-component.component';
import { StatusFilterComponent } from './status-filter/status-filter.component';
import {GsGenericPieComponent} from "../../../generic-widget-content/gs-generic-pie/gs-generic-pie.component";
import { ChartData } from 'chart.js';

@Component({
  selector: 'email-contacts-distribution-graph-widget',
  templateUrl: './email-contacts-distribution-graph-widget.component.html',
  styleUrls: ['./email-contacts-distribution-graph-widget.component.scss'],
  standalone: true,
  imports: [FrameComponent, StatusFilterComponent, NgIf, CommonModule, GSBaseWidget, GsGenericPieComponent]
})
export class EmailContactsDistributionGraphWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  // Data structure for the pie chart
  pieChartData: ChartDataResponse | null = null;

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

      this.emisSoapService.getChartData(chartParams).pipe(
        map(response => {
          console.log(`Fetched IVR OACD Distribution Graph data:`, response);
          return response;
        }),
        catchError(error => {
          console.error(`Error fetching IVR OACD Distribution Graph data for ${this.widget.id}:`, error);
          if (this.widget.state) {
            this.widget.state.error = error.message || 'Failed to fetch chart data';
          }
          return throwError(() => error);
        }),
        finalize(() => {
          if (this.widget.state) {
            this.widget.state.loading = false;
          }
        })
      ).subscribe({
        next: (data) => {
          this.pieChartData = data;
          if (this.widget.state) {
            this.widget.state.data = data; // Store fetched data in widget state
            this.widget.state.lastUpdated = Date.now();
          }
        },
        error: (error) => {
          // Error already handled
        }
      });
    }
  }

  override ngOnDestroy(): void {
    super.ngOnDestroy(); // Call base class OnDestroy
  }
}
