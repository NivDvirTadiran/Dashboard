import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { Observable, catchError, map, throwError } from 'rxjs';
// Assuming DnisListItemDto exists or will be created, similar to Agent related DTOs
// For now, using a generic type or any if the exact DTO is not yet defined.
// import { DnisListItemDto, ReturnMatrixDataTypeBG, DataItemType } from 'src/app/services/emis-soap.service';
import { EmisSoapService, ReturnMatrixDataTypeBG, DataItemType } from 'src/app/services/emis-soap.service'; // Using existing types for now
import { GsGenericTableComponent, TableColumn, TableRowData } from '../../../gs-generic-table/gs-generic-table.component';

@Component({
  selector: 'brief-dnis-widget', // Changed
  templateUrl: './brief-dnis-widget.component.html', // Changed
  styleUrls: ['./brief-dnis-widget.component.scss'], // Changed
  standalone: true,
  imports: [CommonModule, GSBaseWidget, GsGenericTableComponent]
})
export class BriefDnisWidgetComponent extends GSBaseWidget implements OnDestroy { // Changed
  @HostBinding('style.display') display = 'contents';

  tableColumns: TableColumn[] = [ // Updated for DNIS
    { key: "DNIS Name", header: "DNIS Name" },
    { key: "DNIS Number", header: "DNIS Number" },
    { key: "DNIS Type", header: "DNIS Type" },
    // Add more columns as needed based on the actual API response
  ];
  tableData: TableRowData[] = [];


  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService
  ) {
    super(widgetManager);
    console.log('constructor BriefDnisWidgetComponent'); // Changed
  }

  public override fetchData(): void {
    if (!this.widget || !this.widget.state) {
      if (this.widget && this.widget.state) {
          this.widget.state.loading = false;
      }
      return;
    }

    super.fetchData();

    const selectedDnisIds = this.widget.config?.selectedDnisIds; // Changed, assuming this config property
    console.log(`BriefDnisWidget fetchData with selectedDnisIds:`, selectedDnisIds); // Changed

    // Changed to getBriefDnis, assuming selectedDnisIds is the correct parameter
    this.emisSoapService.getBriefDnis(selectedDnisIds).pipe( // Changed service call
        map(data => {
          console.log(`Fetched brief-dnis-data with filter:`, data); // Changed
          if (data && data.returnMatrix) {
            return data.returnMatrix.map(item => {
              const row: TableRowData = {};
              if (item.returnArray && item.returnArray.length > 0 && item.returnArray[item.returnArray.length - 1]?.value) {
                const rawDataString = item.returnArray[item.returnArray.length - 1].value;
                const parsedMap = this.parseDnisDataStringToMap(rawDataString); // Changed
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
          console.error(`Error fetching data for ${this.widget.id} of type ${this.widget.type}:`, error);
          if (this.widget.state) { // Check if state exists
            this.widget.state.error = error.message || 'Failed to fetch data';
            this.widget.state.loading = false; // Ensure loading is false on error
          }
          return throwError(() => error);
        })
      ).subscribe({
        next: (transformedData) => {
          this.tableData = transformedData;
          if (this.widget.state) {
            this.widget.state.data = transformedData;
            this.widget.state.loading = false;
            this.widget.state.lastUpdated = Date.now();
          }
        },
        error: (error) => {
          // Error is already handled in catchError, but ensure loading is false
          if (this.widget.state) {
            this.widget.state.loading = false;
          }
          // Error is already logged by catchError
        }
      });
  }

  private parseDnisDataStringToMap(rawDataString: string): Map<string, string> { // Changed
    const dataMap = new Map<string, string>();
    if (!rawDataString || typeof rawDataString !== 'string') {
      return dataMap;
    }
    const parts = rawDataString.split(',');
    parts.forEach(part => {
      const cell = part.split(';');
      if (cell.length >= 4 && cell[2] !== undefined && cell[3] !== undefined) {
        dataMap.set(cell[2], cell[3]); // Assuming key is cell[2] and value is cell[3]
      }
    });
    return dataMap;
  }

  override ngOnDestroy(): void {
    super.ngOnDestroy();
  }
}
