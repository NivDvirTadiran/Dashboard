import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { catchError, map, throwError } from 'rxjs';
import { EmisSoapService, SuperGroupListItemType } from 'src/app/services/emis-soap.service';
import { GsGenericTableComponent, TableColumn, TableRowData } from '../../../gs-generic-table/gs-generic-table.component';

@Component({
  selector: 'super-group-list-widget',
  templateUrl: './super-group-list-widget.component.html',
  styleUrls: ['./super-group-list-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, GSBaseWidget, GsGenericTableComponent]
})
export class SuperGroupListWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  tableColumns: TableColumn[] = [
    { key: 'superGroupId', header: 'Group ID' },
    { key: 'superGroupName', header: 'Name' }
  ];
  tableData: TableRowData[] = [];

  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService,
    public override emisSoapService: EmisSoapService
  ) {
    super(widgetManager);
    console.log('constructor SuperGroupListWidgetComponent');
  }

  public override fetchData(): void {
    if (this.widget && this.widget.state) {
      this.widget.state.loading = true;
      this.widget.state.error = undefined;

      this.emisSoapService.getSuperGroupList().pipe(
        map(response => { // Changed 'data' to 'response' to avoid confusion with widget.state.data
          console.log(`Fetched super-group-list-table:`, response);
          // Assuming response is SuperGroupListReturnType
          return (response.returnArray || []).map(item => ({
            superGroupId: item.superGroupId,
            superGroupName: item.superGroupName
            // Note: item.returnArray (nested groups) is not handled here for simplicity.
            // If needed, this transformation would be more complex.
          }));
        }),
        catchError(err => {
          console.error('Error fetching super-group-list-table data in manager:', err);
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
