import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../base-widget/base-widget.component';
import { Observable, throwError } from 'rxjs';
import { catchError, map } from 'rxjs/operators';
import { EmisSoapService, ChartRequestParams } from 'src/app/services/emis-soap.service';
import { DomSanitizer, SafeUrl } from '@angular/platform-browser';

@Component({
  selector: 'chart-widget',
  templateUrl: './chart-widget.component.html',
  styleUrls: ['./chart-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, GSBaseWidget]
})
export class ChartWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';
  chartImageUrl: SafeUrl | null = null;

  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService,
    public override emisSoapService: EmisSoapService,
    private sanitizer: DomSanitizer
  ) {
    super(widgetManager);
    console.log('constructor ChartWidgetComponent');
  }

  public override fetchData(): void {
    if (this.widget && this.widget.state) {
      this.widget.state.loading = true;
      this.widget.state.error = undefined;
      this.chartImageUrl = null; // Clear previous image

      const chartParams: ChartRequestParams = this.widget.settings || {};

      this.emisSoapService.getChart(chartParams).pipe(
        map(blob => {
          console.log(`Fetched chart data for widget ${this.widget.id}:`, blob);
          return blob;
        }),
        catchError(err => {
          console.error(`Error fetching chart data for widget ${this.widget.id} in component:`, err);
          this.widget.state!.error = err.message || 'Failed to fetch chart data';
          return throwError(() => err);
        })
      ).subscribe({
        next: (dataBlob: Blob) => {
          if (this.widget.state) {
            this.updateChartImage(dataBlob);
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
          this.chartImageUrl = null;
          console.error(`Widget ${this.widget.id} update failed:`, error);
        }
      });
    }
  }

  private updateChartImage(blob: Blob): void {
    if (this.chartImageUrl) {
      URL.revokeObjectURL(this.chartImageUrl.toString());
    }
    const objectURL = URL.createObjectURL(blob);
    this.chartImageUrl = this.sanitizer.bypassSecurityTrustUrl(objectURL);
  }

  override ngOnDestroy(): void {
    super.ngOnDestroy(); // Call base class OnDestroy
    if (this.chartImageUrl) {
      URL.revokeObjectURL(this.chartImageUrl.toString());
    }
  }
}
