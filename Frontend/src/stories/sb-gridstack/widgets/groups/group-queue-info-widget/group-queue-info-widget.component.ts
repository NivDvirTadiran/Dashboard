import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { catchError, map, throwError } from 'rxjs';
import { EmisSoapService, GroupDetailInfoDataItemType, DataItemType } from 'src/app/services/emis-soap.service';
import { GsGenericTableComponent, TableColumn, TableRowData } from '../../../gs-generic-table/gs-generic-table.component';

@Component({
  selector: 'group-queue-info-widget',
  templateUrl: './group-queue-info-widget.component.html',
  styleUrls: ['./group-queue-info-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, GSBaseWidget, GsGenericTableComponent]
})
export class GroupQueueInfoWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  tableColumns: TableColumn[] = [
    { key: 'name', header: 'Name' },
    { key: 'value', header: 'Value' }
  ];
  tableData: TableRowData[] = [];

  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService,
    public override emisSoapService: EmisSoapService
  ) {
    super(widgetManager);
    console.log('constructor GroupQueueInfoWidgetComponent');
  }

  public override fetchData(): void {
    if (this.widget && this.widget.state) {
      this.widget.state.loading = true;
      this.widget.state.error = undefined;

      const groupId = this.widget.settings?.['groupId'];
      if (!groupId) {
        console.error('Group ID is required for group-queue-info-table widget');
        this.widget.state.error = 'Group ID is required';
        this.widget.state.loading = false;
        return;
      }

      this.emisSoapService.getGroupQueInfo(groupId).pipe(
        map((response: GroupDetailInfoDataItemType[]) => { // Changed data to response
          console.log(`Fetched group-queue-info-table:`, response);
          if (response && response.length > 0 && response[0].returnArray) {
            return response[0].returnArray.map((item: DataItemType) => ({
              name: item.ersname,
              value: item.value
            }));
          }
          return [];
        }),
        catchError(err => {
          console.error('Error fetching group-queue-info-table data in manager:', err);
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
