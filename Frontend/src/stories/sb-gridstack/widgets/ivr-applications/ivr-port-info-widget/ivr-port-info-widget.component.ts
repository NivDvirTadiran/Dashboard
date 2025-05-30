import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { catchError, map, throwError } from 'rxjs';
import { EmisSoapService, DataItemType } from 'src/app/services/emis-soap.service';
import { GsGenericTableComponent, TableColumn, TableRowData } from '../../../gs-generic-table/gs-generic-table.component';

@Component({
  selector: 'ivr-port-info-widget',
  templateUrl: './ivr-port-info-widget.component.html',
  styleUrls: ['./ivr-port-info-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, GSBaseWidget, GsGenericTableComponent]
})
export class IvrPortInfoWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  tableColumns: TableColumn[] = [
    { key: 'name', header: 'Name' },
    { key: 'value', header: 'Value' }
  ];
  tableData: TableRowData[] = [];

  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService
    // Remove private emisSoapService, will use super.emisSoapService from GSBaseWidget
  ) {
    super(widgetManager);
    console.log('constructor IvrPortInfoWidgetComponent');
  }

  // Make fetchData public to match GSBaseWidget
  public override fetchData(): void {
    if (!this.widget || !this.widget.state) {
      if (this.widget && this.widget.state) {
          this.widget.state.loading = false;
      }
      return;
    }

    super.fetchData(); // Handles setting loading to true

    const portId = (this.widget.config as any)?.portId || this.widget.settings?.['portId']; // Check config first
    if (!portId) {
      console.error('Port ID is required for ivr-port-info-widget');
      if (this.widget.state) {
        this.widget.state.error = 'Port ID is required';
        this.widget.state.loading = false;
      }
      return;
    }

    // Use this.emisSoapService from GSBaseWidget
    this.emisSoapService.getIvrPortInfo(portId).pipe(
        map((data: DataItemType[]) => { // Assuming getIvrPortInfo returns DataItemType[] directly
          console.log(`Fetched ivr-port-info-table for portId ${portId}:`, data);
          return (data || []).map(item => ({ // Ensure data is an array before mapping
            name: item.ersname, // ersname might be optional
            value: item.value   // value might be optional
          }));
        }),
        catchError(err => {
          console.error('Error fetching ivr-port-info-table data:', err);
          if (this.widget.state) {
            this.widget.state.error = err.message || 'Failed to fetch data';
            this.widget.state.loading = false;
          }
          return throwError(() => err);
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
        error: (error) => { // Error is already handled in catchError
          if (this.widget.state) {
            this.widget.state.loading = false;
          }
          // Error is already logged
        }
      });
  }

  override ngOnDestroy(): void {
    super.ngOnDestroy();
  }
}
