import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { catchError, map, throwError } from 'rxjs';
import { EmisSoapService, DataItemType } from 'src/app/services/emis-soap.service';
import { GsGenericTableComponent, TableColumn, TableRowData } from '../../../generic-widget-content/gs-generic-table/gs-generic-table.component';

@Component({
  selector: 'group-agents-info-widget',
  templateUrl: './group-agents-info-widget.component.html',
  styleUrls: ['./group-agents-info-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, GSBaseWidget, GsGenericTableComponent]
})
export class GroupAgentsInfoWidgetComponent extends GSBaseWidget implements OnDestroy {
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
    console.log('constructor GroupAgentsInfoWidgetComponent');
  }

  public override fetchData(): void {
    if (this.widget && this.widget.state) {
      this.widget.state.loading = true;
      this.widget.state.error = undefined;

      const groupId = this.widget.settings?.['groupId'];
      if (!groupId) {
        console.error('Group ID is required for group-agents-info-widget');
        this.widget.state.error = 'Group ID is required';
        this.widget.state.loading = false;
        return;
      }

      this.emisSoapService.getGroupAgentsInfo(groupId).pipe(
        map((data: DataItemType[]) => {
          console.log(`Fetched group-agents-info-table:`, data);
          return (data || []).map(item => ({
            name: item.ersname,
            value: item.value
          }));
        }),
        catchError(err => {
          console.error('Error fetching group-agents-info-table data in manager:', err);
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
