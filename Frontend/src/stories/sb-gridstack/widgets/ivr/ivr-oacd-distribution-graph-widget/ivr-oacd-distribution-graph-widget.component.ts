import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { GsDashboardWidgetManagerService } from '../../../gs-dashboard-widget-manager.service';
import { EmisSoapService, ChartDataResponse, GenListDataItemType } from 'src/app/services/emis-soap.service'; // Assuming ChartDataResponse is suitable
import { WidgetHeaderComponent } from '../../../widget-header/widget-header.component';
import { WidgetContentWrapperComponent } from '../../../widget-content-wrapper/widget-content-wrapper.component';
import { WidgetFooterComponent } from '../../../widget-footer/widget-footer.component';
import { WidgetSettingsModalComponent, SelectItem } from '../../../widget-settings-modal/widget-settings-modal.component';
import { GsGenericPieComponent } from '../../../generic-widget-content/gs-generic-pie/gs-generic-pie.component'; // Import Pie component
import { catchError, finalize, map, throwError } from 'rxjs';

@Component({
  selector: 'ivr-oacd-distribution-graph-widget', // Unique selector
  standalone: true,
  imports: [
    CommonModule,
    GsGenericPieComponent, // Use Pie component
    WidgetHeaderComponent,
    WidgetContentWrapperComponent,
    WidgetFooterComponent,
    WidgetSettingsModalComponent
  ],
  templateUrl: './ivr-oacd-distribution-graph-widget.component.html',
  styleUrls: ['./ivr-oacd-distribution-graph-widget.component.scss']
})
export class IvrOacdDistributionGraphWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  chartData: ChartDataResponse | null = null;

  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService
  ) {
    super(widgetManager);
    console.log('constructor IvrOacdDistributionGraphWidgetComponent');
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
    let refObjectName = 'IVRApplications'; // Default or determined by selection
    if (selectedEntitiesIds && selectedEntitiesIds.length > 0) {
      // Assuming the first selected ID is the IVR Application ID for this graph
      refObjectName = `IVRApplication:${selectedEntitiesIds[0]}`;
    } else {
      // Handle case where no specific IVR application is selected,
      // maybe fetch a system-wide graph or show an error/prompt.
      // For now, we might use a default or a generic identifier if the backend supports it.
      // This depends on the backend's capabilities for "IVR OACD Distribution Graph".
      console.warn('No specific IVR Application ID selected for IVR OACD Distribution Graph. Fetching generic or default.');
      // refObjectName might need to be set to a generic value or the backend handles it.
    }

    // Parameters for "IVR OACD Distribution Graph"
    // These need to be determined based on backend requirements for this specific graph.
    // The `title` from add-widget-menu.component.ts is "IVR OACD Distribution Graph".
    // The `contentType` is "pie".
    const chartParams = {
      type: 'pie', // Or '3Dpie' or whatever the backend expects for this graph
      title: 'IVR_OACD_Distribution_Graph', // This should match the backend's expected title for data fetching
      keys: 'status,count', // Placeholder: Actual keys depend on the data structure for this graph
      refObjectName: refObjectName, // e.g., "IVRApplication:1" or a general one
      // size might be handled by the generic component or set here if needed
    };

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
        this.chartData = data;
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

  protected override prepareModalData() {
    super.prepareModalData();
    this.loadIvrApplicationsForSelection();
  }

  private loadIvrApplicationsForSelection(): void {
    if (!this.widget || this.widget.type !== 'ivr-oacd-distribution-graph-widget') { // Match selector
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
            id: app.id,
            name: app.name,
            selected: selectedIds.has(String(app.id))
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
