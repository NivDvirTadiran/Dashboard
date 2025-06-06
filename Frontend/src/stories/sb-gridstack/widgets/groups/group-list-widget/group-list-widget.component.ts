import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { catchError, map, throwError } from 'rxjs';
import { EmisSoapService, GroupListDataItemType } from 'src/app/services/emis-soap.service';
import { GsGenericTableComponent, TableColumn, TableRowData } from '../../../generic-widget-content/gs-generic-table/gs-generic-table.component';

@Component({
  selector: 'group-list-widget',
  templateUrl: './group-list-widget.component.html',
  styleUrls: ['./group-list-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, GSBaseWidget, GsGenericTableComponent]
})
export class GroupListWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  tableColumns: TableColumn[] = [
    { key: 'grpId', header: 'Group ID' },
    { key: 'grpName', header: 'Name' },
    { key: 'grpNumber', header: 'Number' },
    { key: 'grpEmailEnable', header: 'Email Enabled' }
  ];
  tableData: TableRowData[] = [];

  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService,
    public override emisSoapService: EmisSoapService
  ) {
    super(widgetManager);
    console.log('constructor GroupListWidgetComponent');
  }

  public override fetchData(): void {
    if (this.widget && this.widget.state) {
      this.widget.state.loading = true;
      this.widget.state.error = undefined;

      this.emisSoapService.getGroupList().pipe(
        map((data: GroupListDataItemType[]) => {
          console.log(`Fetched group-list-table:`, data);
          return (data || []).map(item => ({
            grpId: item.grpId,
            grpName: item.grpName,
            grpNumber: item.grpNumber,
            grpEmailEnable: item.grpEmailEnable === 1 ? 'Yes' : 'No' // Example transformation
          }));
        }),
        catchError(err => {
          console.error('Error fetching group-list-table data in manager:', err);
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
