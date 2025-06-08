import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { GsDashboardWidgetManagerService } from '../../../gs-dashboard-widget-manager.service';
import { EmisSoapService, ChartRequestParams, ChartDataResponse, ChartDataset, GenListDataItemType } from 'src/app/services/emis-soap.service';
import { GsGenericGraphComponent } from '../../../generic-widget-content/gs-generic-graph/gs-generic-graph.component'; // Import Graph component
import { WidgetHeaderComponent } from '../../../widget-header/widget-header.component';
import { WidgetContentWrapperComponent } from '../../../widget-content-wrapper/widget-content-wrapper.component';
import { WidgetFooterComponent } from '../../../widget-footer/widget-footer.component';
import { WidgetSettingsModalComponent, SelectItem } from '../../../widget-settings-modal/widget-settings-modal.component';
import { catchError, finalize, map, throwError } from 'rxjs';

@Component({
  selector: 'ivr-active-acd-graph-widget', // Unique selector
  standalone: true,
  imports: [
    CommonModule,
    GsGenericGraphComponent, // Use Graph component
    WidgetHeaderComponent,
    WidgetContentWrapperComponent,
    WidgetFooterComponent,
    WidgetSettingsModalComponent
  ],
  templateUrl: './ivr-active-acd-graph-widget.component.html',
  styleUrls: ['./ivr-active-acd-graph-widget.component.scss']
})
export class IvrActiveAcdGraphWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  // Data structure for the graph (matches ChartDataResponse from service)
  chartData: ChartDataResponse | null = null;

  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService
  ) {
    super(widgetManager);
    console.log('constructor IvrActiveAcdGraphWidgetComponent');
  }

  public override fetchData(): void {
    if (!this.widget || !this.widget.state) {
      if (this.widget && this.widget.state) {
        this.widget.state.loading = false;
      }
      return;
    }

    super.fetchData(); // Sets loading to true

    const selectedEntitiesIds = this.widget.config?.selectedEntitiesIds;
    const chartSettings = this.widget.settings;

    console.log(`IvrActiveAcdGraphWidget fetchData with selectedIvrAppIds:`, selectedEntitiesIds, `and settings:`, chartSettings);

    const params: ChartRequestParams = {
      refObjectName: selectedEntitiesIds && selectedEntitiesIds.length > 0 ? `IvrApplication:${selectedEntitiesIds.join(',')}` : 'IvrApplication:All', // Example
      type: 'graph', // Or a specific type identifier for this chart
      title: this.widget.title || 'IVR Active ACD Graph', // Use widget title
      keys: chartSettings?.keys || 'time,value', // Example keys, adjust as needed
      ...chartSettings
    };

    this.emisSoapService.getChartData(params).pipe(
      map((response: ChartDataResponse) => {
        console.log(`Fetched IVR Active ACD graph data:`, response);
        // gs-generic-graph expects ChartDataResponse directly
        return response;
      }),
      catchError(error => {
        console.error(`Error fetching chart data for ${this.widget.id}:`, error);
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
      next: (data: ChartDataResponse) => {
        this.chartData = data;
        if (this.widget.state) {
          this.widget.state.data = data;
          this.widget.state.lastUpdated = Date.now();
        }
      },
      error: (error) => {
        // Error already handled by catchError
      }
    });
  }

  protected override prepareModalData() {
    super.prepareModalData();
    this.loadIvrApplicationsForSelection();
  }

  private loadIvrApplicationsForSelection(): void {
    if (!this.widget || this.widget.type !== 'ivr-active-acd-graph-widget') { // Match selector
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
