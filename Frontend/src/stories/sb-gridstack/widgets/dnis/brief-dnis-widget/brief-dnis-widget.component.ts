import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { Observable, catchError, finalize, map, tap, throwError } from 'rxjs';
import {
  EmisSoapService,
  ReturnMatrixDataTypeBG,
  DataItemType,
  DataItemRow,
  DnisListItemType
} from 'src/app/services/emis-soap.service';
import { GsGenericTableComponent, TableColumn, TableRowData } from '../../../generic-widget-content/gs-generic-table/gs-generic-table.component';
import { WidgetHeaderComponent } from '../../../widget-header/widget-header.component';
import { WidgetContentWrapperComponent } from '../../../widget-content-wrapper/widget-content-wrapper.component';
import { WidgetFooterComponent } from '../../../widget-footer/widget-footer.component';
import { WidgetSettingsModalComponent } from '../../../widget-settings-modal/widget-settings-modal.component';
import {GSBaseWidget} from "../../../base-widget/base-widget.component";

@Component({
  selector: 'brief-dnis-widget',
  templateUrl: './brief-dnis-widget.component.html',
  styleUrls: ['./brief-dnis-widget.component.scss'],
  standalone: true,
  imports: [
    CommonModule,
    GsGenericTableComponent,
    WidgetHeaderComponent,
    WidgetContentWrapperComponent,
    WidgetFooterComponent,
    WidgetSettingsModalComponent
  ]
})
export class BriefDnisWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  tableColumns: TableColumn[] = [
    { key: "DNIS Id", header: "DNIS Id" },
    { key: "DNIS Name", header: "DNIS Name" },
    { key: "DNIS No.", header: "DNIS No." },
    { key: "Above T.ASA", header: "Above T.ASA" },
    { key: "Calls In Q", header: "Calls In Q" },
    { key: "Max Time In Q", header: "Max Time In Q" },
    { key: "Calls ACD", header: "Calls ACD" },
    { key: "Calls Ansd", header: "Calls Ansd" },
    { key: "Calls Abnd", header: "Calls Abnd" },
    { key: "TSF", header: "TSF" },
    { key: "Avg. Wait Abnd", header: "Avg. Wait Abnd" },
    { key: "Max Wait Abnd", header: "Max Wait Abnd" },
    { key: "Avg. Q Time", header: "Avg. Q Time" },
    { key: "Max Q Time", header: "Max Q Time" },
  ];
  tableData: TableRowData[] = [];






  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService
  ) {
    super(widgetManager);
    console.log('constructor BriefDnisWidgetComponent');
  }

  public override fetchData(): void {
    console.log('[BriefDnisWidgetComponent] fetchData: CALLED');
    if (!this.widget || !this.widget.state) {
      if (this.widget && this.widget.state) {
          this.widget.state.loading = false;
      }
      console.log('[BriefDnisWidgetComponent] fetchData: SKIPPED - no widget or state');
      return;
    }

    super.fetchData();

    // Correctly access selectedDnisIds from widget.config.selectedDnisIds
    const selectedDnisIds = this.widget.config?.selectedDnisIds;
    console.log(`BriefDnisWidget fetchData with selectedDnisIds:`, selectedDnisIds);

    // Changed to getBriefDnis, assuming selectedDnisIds is the correct parameter
    this.emisSoapService.getBriefDnis(selectedDnisIds).pipe( // Changed service call
        map(data => {
          console.log(`Fetched brief-dnis-data with filter:`, data); // Changed
          if (data && data.returnMatrix) {
            return data.returnMatrix.map(item => {
              const row: TableRowData = {};
              if (item.returnArray && item.returnArray.length > 0 && item.returnArray[item.returnArray.length - 1]?.value) {
                const rawData: DataItemType[] = item.returnArray;
                const parsedMap = this.parseDnisDataItemTypeArrayToMap(rawData); // Changed
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
          console.error(`[BriefDnisWidgetComponent] Error fetching data (in catchError): ${this.widget.id} of type ${this.widget.type}:`, error);
          if (this.widget.state) {
            this.widget.state.error = error.message || 'Failed to fetch data';
            this.widget.state.loading = false;
          }
          return throwError(() => error);
        })
      ).subscribe({
        next: (transformedData) => {
          console.log('[BriefDnisWidgetComponent] Data transformed (subscribe next):', transformedData);
          this.tableData = transformedData;
          if (this.widget.state) {
            this.widget.state.data = transformedData;
            this.widget.state.loading = false;
            this.widget.state.lastUpdated = Date.now();
          }
        },
        error: (error) => {
          console.error('[BriefDnisWidgetComponent] Subscribe error:', error);
          if (this.widget.state) {
            this.widget.state.loading = false;
          }
        }
      });
  }

  private parseDnisDataItemTypeArrayToMap(rawDataItemTypeArray: DataItemType[]): Map<string, string> {
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
    this.loadDnisForSelection();
  }

  private loadDnisForSelection(): void {
    console.log('[BriefDnisWidgetComponent] loadDnisForSelection: ENTERED');
    if (!this.widget || this.widget.type !== 'brief-dnis-widget') {
      console.warn('[BriefDnisWidgetComponent] loadDnisForSelection: Invalid widget or widget type. Widget:', this.widget);
      return;
    }

    this.isLoading = true;
    this.emisSoapService.getDnisList().pipe(
      tap(dnisList => console.log('DnisList raw response:', dnisList)),
      finalize(() => this.isLoading = false)
    ).subscribe({
      next: (dnisList: DnisListItemType[]) => {
        if (dnisList && Array.isArray(dnisList)) {
          const selectedIds = new Set(this.widget.config?.selectedEntitiesIds || []);
          this.entitiesForSelection = dnisList.map((dnis: DnisListItemType) => ({
            id: dnis.dnisId.toString(),
            name: dnis.dnisName,
            selected: selectedIds.has(dnis.dnisId.toString())
          }));
        } else {
          this.entitiesForSelection = [];
          console.warn('No DNIS returned or invalid format for brief-dnis-widget selection.');
        }
      },
      error: (err) => {
        console.error('Error loading DNIS for selection:', err);
        this.entitiesForSelection = [];
      }
    });
  }

  override ngOnDestroy(): void {
    super.ngOnDestroy();
  }
}
