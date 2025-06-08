import { Component, HostBinding, OnDestroy, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { Observable, catchError, finalize, map, tap, throwError } from 'rxjs';
import {
  EmisSoapService,
  ChartDataResponse,
  ChartRequestParams, // Added ChartRequestParams
  ChartDataset, // Added ChartDataset
  GenListDataItemType // For IVR App List
} from 'src/app/services/emis-soap.service';
import { ChartData } from 'chart.js'; // Import ChartData
import { GsGenericPieComponent } from '../../../generic-widget-content/gs-generic-pie/gs-generic-pie.component'; // Import PieChartDataItem
import { WidgetSettingsModalComponent, SelectItem } from '../../../widget-settings-modal/widget-settings-modal.component';
import { WidgetHeaderComponent } from '../../../widget-header/widget-header.component';
import { WidgetContentWrapperComponent } from '../../../widget-content-wrapper/widget-content-wrapper.component';
import { WidgetFooterComponent } from '../../../widget-footer/widget-footer.component';

@Component({
  selector: 'ivr-acd-distribution-graph-widget', // Unique selector
  standalone: true,
  imports: [
    CommonModule,
    GsGenericPieComponent, // Use Pie component
    WidgetSettingsModalComponent,
    WidgetHeaderComponent,
    WidgetContentWrapperComponent,
    WidgetFooterComponent
  ],
  templateUrl: './ivr-acd-distribution-graph-widget.component.html',
  styleUrls: ['./ivr-acd-distribution-graph-widget.component.scss']
})
export class IvrAcdDistributionGraphWidgetComponent extends GSBaseWidget implements OnDestroy, OnInit {
  @HostBinding('style.display') display = 'contents';

  // Data structure for the pie chart
  pieChartData: ChartDataResponse | null = null;

  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService
  ) {
    super(widgetManager);
    console.log('constructor IvrAcdDistributionGraphWidgetComponent');
  }

  public override fetchData(): void {
    if (!this.widget || !this.widget.state) {
      if (this.widget && this.widget.state) {
        this.widget.state.loading = false;
      }
      return;
    }

    super.fetchData();

    const selectedEntitiesIds = this.widget.config?.selectedEntitiesIds;
    const chartSettings = this.widget.settings; // Specific settings for this chart

    console.log(`IvrAcdDistributionGraphWidget fetchData with selectedIvrAppIds:`, selectedEntitiesIds, `and settings:`, chartSettings);

    // TODO: Replace with actual service call for this specific chart data
    // This might involve a new method in EmisSoapService like getIvrAcdDistributionData
    // For now, using a placeholder or adapting getChartData if suitable
    const params: ChartRequestParams = { // Defined params explicitly
      refObjectName: selectedEntitiesIds && selectedEntitiesIds.length > 0 ? `IvrApplication:${selectedEntitiesIds.join(',')}` : 'IvrApplication:All',
      type: 'pie',
      title: this.widget.title || 'IVR ACD Distribution Graph',
      keys: chartSettings?.keys || 'status,count',
      ...chartSettings
    };

    this.emisSoapService.getChartData(params).pipe(
      map(response => {
        console.log(`Fetched IVR ACD Distribution graph data:`, response);
        if (response && response.labels && response.datasets && response.datasets.length > 0) {
          // Assuming the first dataset is relevant for the pie chart
          const firstDataset = response.datasets[0];
          return {
            labels: response.labels,
            datasets: [
              {
                data: firstDataset.data,
                backgroundColor: firstDataset.backgroundColor,
                // label: firstDataset.label // Optional: Pie charts usually don't show dataset label
              }
            ]
          };
        }
        // Return a default/empty ChartData structure if the response is not valid
        return { labels: [], datasets: [{ data: [], backgroundColor: [] }] };
      }),
      catchError(error => {
        console.error(`Error fetching data for ${this.widget.id}:`, error);
        if (this.widget.state) {
          this.widget.state.error = error.message || 'Failed to fetch chart data';
          this.widget.state.loading = false;
        }
        return throwError(() => error);
      })
    ).subscribe({
      next: (chartData) => {
        this.pieChartData = chartData;
        if (this.widget.state) {
          this.widget.state.data = chartData; // Store ChartData
          this.widget.state.loading = false;
          this.widget.state.lastUpdated = Date.now();
        }
      },
      error: (error) => {
        if (this.widget.state) {
          this.widget.state.loading = false;
        }
      }
    });
  }

  protected override prepareModalData() {
    super.prepareModalData();
    this.loadIvrApplicationsForSelection();
  }

  private loadIvrApplicationsForSelection(): void {
    if (!this.widget || this.widget.type !== 'ivr-acd-distribution-graph-widget') { // Match selector
      return;
    }
    this.isLoading = true;
    this.emisSoapService.getIvrAppList().pipe(
      finalize(() => this.isLoading = false)
    ).subscribe({
      next: (appList: GenListDataItemType[]) => {
        if (appList && Array.isArray(appList)) {
          const selectedIds = new Set(this.widget.config?.selectedEntitiesIds || []);
          this.entitiesForSelection = appList.map((app: GenListDataItemType) => ({
            id: app.id.toString(),
            name: app.name,
            selected: selectedIds.has(app.id.toString())
          }));
        } else {
          this.entitiesForSelection = [];
        }
      },
      error: (err) => {
        console.error('Error loading IVR Applications for selection:', err);
        this.entitiesForSelection = [];
      }
    });
  }

  override ngOnDestroy(): void {
    super.ngOnDestroy();
  }
}
