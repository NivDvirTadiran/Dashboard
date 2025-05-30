import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { catchError, map, throwError } from 'rxjs';
import { EmisSoapService, DnisListItemType } from 'src/app/services/emis-soap.service';
import { GsGenericTableComponent, TableColumn, TableRowData } from '../../../gs-generic-table/gs-generic-table.component';

@Component({
  selector: 'dnis-list-widget',
  templateUrl: './dnis-list-widget.component.html',
  styleUrls: ['./dnis-list-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, GSBaseWidget, GsGenericTableComponent]
})
export class DnisListWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  tableColumns: TableColumn[] = [
    { key: 'dnisId', header: 'DNIS ID' },
    { key: 'dnisName', header: 'Name' }
  ];
  tableData: TableRowData[] = [];

  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService,
    public override emisSoapService: EmisSoapService
  ) {
    super(widgetManager);
    console.log('constructor DnisListWidgetComponent');
  }

  public override fetchData(): void {
    if (this.widget && this.widget.state) {
      this.widget.state.loading = true;
      this.widget.state.error = undefined;

      this.emisSoapService.getDnisList().pipe(
        map((data: DnisListItemType[]) => {
          console.log(`Fetched dnis-list-table:`, data);
          return (data || []).map(item => ({
            dnisId: item.dnisId,
            dnisName: item.dnisName
          }));
        }),
        catchError(err => {
          console.error('Error fetching dnis-list-table data in manager:', err);
          this.widget.state!.error = err.message || 'Failed to fetch data';
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
    super.ngOnDestroy();
  }
}
