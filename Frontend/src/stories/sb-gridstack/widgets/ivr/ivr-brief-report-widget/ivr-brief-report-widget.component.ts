import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { GsDashboardWidgetManagerService } from '../../../gs-dashboard-widget-manager.service';
import { EmisSoapService, ReturnMatrixDataTypeBG, DataItemType, GenListDataItemType } from 'src/app/services/emis-soap.service';
import { GsGenericTableComponent, TableColumn, TableRowData } from '../../../generic-widget-content/gs-generic-table/gs-generic-table.component';
import { WidgetHeaderComponent } from '../../../widget-header/widget-header.component';
import { WidgetContentWrapperComponent } from '../../../widget-content-wrapper/widget-content-wrapper.component';
import { WidgetFooterComponent } from '../../../widget-footer/widget-footer.component';
import { WidgetSettingsModalComponent, SelectItem } from '../../../widget-settings-modal/widget-settings-modal.component';
import { catchError, finalize, map, throwError } from 'rxjs';

@Component({
  selector: 'ivr-brief-report-widget', // Unique selector
  standalone: true,
  imports: [
    CommonModule,
    GsGenericTableComponent,
    WidgetHeaderComponent,
    WidgetContentWrapperComponent,
    WidgetFooterComponent,
    WidgetSettingsModalComponent
  ],
  templateUrl: './ivr-brief-report-widget.component.html',
  styleUrls: ['./ivr-brief-report-widget.component.scss']
})
export class IvrBriefReportWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  tableColumns: TableColumn[] = [

    { key: "IVR App. Name", header: "IVR App. Name" },
    { key: "Interval Time", header: "Interval Time" },
    { key: "Calls", header: "Calls" },
    { key: "Too Long ACD", header: "Too Long ACD" },
    { key: "Avg.time", header: "Avg.time" },
    { key: "Max time", header: "Max time" },
    { key: "RPH ACD", header: "RPH ACD" },
    { key: "Times Accessed ACD", header: "Times Accessed ACD" },
    { key: "Fully Handled ACD", header: "Fully Handled ACD" },
    { key: "Failed ACD",  header: "Failed ACD", },
    { key: "Interflowed ACD", header: "Interflowed ACD" },
    { key: "All/access",  header: "All/access", },
    { key: "Max Handled ACD", header: "Max Handled ACD" },
    { key: "Fully handled", header: "Fully handled" },
    { key: "longest_fully_handle_time", header: "longest_fully_handle_time" },
    { key: "Avg. Failed", header: "Avg. Failed" },
    { key: "Failed", header: "Failed" },
    { key: "Avg. Interflowed", header: "Avg. Interflowed" },
    { key: "Long.Interflowed", header: "Long.Interflowed" },
    { key: "Calls OACD",  header: "Calls OACD", },
    { key: "Too Long OACD", header: "Too Long OACD" },
    { key: "Avg.time OACD", header: "Avg.time OACD" },
    { key: "Max time OACD", header: "Max time OACD" },
    { key: "RPH OACD", header: "RPH OACD" },
    { key: "Times Accessed OACD", header: "Times Accessed OACD" },
    { key: "Fully Handled OACD", header: "Fully Handled OACD" },
    { key: "Failed OACD", header: "Failed OACD" },
    { key: "Interflowed OACD", header: "Interflowed OACD" },
    { key: "All/access",  header: "All/access", },
    { key: "Max Handled OACD", header: "Max Handled OACD" },
    { key: "Fully handled", header: "Fully handled" },
    { key: "longest_fully_handle_time", header: "longest_fully_handle_time" },
    { key: "Avg. Failed", header: "Avg. Failed" },
    { key: "Failed", header: "Failed" },
    { key: "Avg. Interflowed", header: "Avg. Interflowed" },
    { key: "Long.Interflowed", header: "Long.Interflowed" }
  ];
  tableData: TableRowData[] = [];







  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService
  ) {
    super(widgetManager);
    console.log('constructor IvrBriefReportWidgetComponent');
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
    console.log(`IvrBriefReportWidget fetchData with selectedIvrAppIds:`, selectedEntitiesIds);


    this.emisSoapService.getIvrBriefReport(selectedEntitiesIds).pipe(
      map(data => {
        console.log(`Fetched IVR Brief Report data with filter:`, data); // Changed
        if (data && data.returnMatrix) {
          return data.returnMatrix.map(item => {
            const row: TableRowData = {};
            if (item.returnArray && item.returnArray.length > 0 && item.returnArray[item.returnArray.length - 1]?.value) {
              const rawData: DataItemType[] = item.returnArray;
              const parsedMap = this.parseIvrDataItemTypeArrayToMap(rawData); // Changed
              this.tableColumns.forEach(col => {
                row[col.key] = parsedMap.get(col.key) || '';
              });
            }
            return row;
          });
        }
        return [];
        }),
        catchError(error => {
          console.error(`Error fetching IVR Brief Report data for ${this.widget.id}:`, error);
          if (this.widget.state) {
            this.widget.state.error = error.message || 'Failed to fetch data';
          }
          return throwError(() => error);
        }),
        finalize(() => {
          if (this.widget.state) {
            this.widget.state.loading = false;
          }
        })
      ).subscribe({
        next: (transformedData) => {
          this.tableData = transformedData;
          if (this.widget.state) {
            this.widget.state.data = transformedData;
            this.widget.state.lastUpdated = Date.now();
          }
        },
        error: (error) => {
          // Error already handled
        }
      });
  }

  private parseIvrDataItemTypeArrayToMap(rawDataItemTypeArray: DataItemType[]): Map<string, string> {
    const dataMap = new Map<string, string>();
    if (!rawDataItemTypeArray) {
      return dataMap;
    }
    rawDataItemTypeArray.forEach((cell: DataItemType) => {
      if (cell.ersname && cell.value) {
        dataMap.set(cell.ersname, cell.value);
      }
    });
    return dataMap;
  }


  protected override prepareModalData() {
    super.prepareModalData();
    this.loadIvrApplicationsForSelection();
  }

  private loadIvrApplicationsForSelection(): void {
    if (!this.widget || this.widget.type !== 'ivr-brief-report-widget') { // Match selector
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
