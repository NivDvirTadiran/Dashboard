import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { catchError, map, throwError } from 'rxjs';
import { EmisSoapService, BlockItemType, DataItemType } from 'src/app/services/emis-soap.service';
import { LoadingIndicatorComponent } from '../../../loading/loading-indicator.component';
import { GsGenericTableComponent, TableColumn, TableRowData } from '../../../gs-generic-table/gs-generic-table.component';

@Component({
  selector: 'group-abandoned-info-widget',
  templateUrl: './group-abandoned-info-widget.component.html',
  styleUrls: ['./group-abandoned-info-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, /*LoadingIndicatorComponent,*/ GSBaseWidget, GsGenericTableComponent]
})
export class GroupAbandonedInfoWidgetComponent extends GSBaseWidget implements OnDestroy {
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
    console.log('constructor GroupAbandonedInfoWidgetComponent');
  }

  public override fetchData(): void {
    if (this.widget && this.widget.state) {
      this.widget.state.loading = true;
      this.widget.state.error = undefined;

      const groupId = this.widget.settings?.['groupId'];
      if (!groupId) {
        console.error('Group ID is required for group-abandoned-info-widget');
        this.widget.state.error = 'Group ID is required';
        this.widget.state.loading = false;
        return;
      }

      this.emisSoapService.getGroupAbandonedInfo(groupId).pipe(
        map(response => { // Changed data to response
          console.log(`Fetched group-abandoned-info-table:`, response);
          // The data is nested within returnArrayOfBlocks, and then each block has a returnArray
          // This widget seems to display a list of key-value pairs from the first block's returnArray
          if (response && response.returnArrayOfBlocks && response.returnArrayOfBlocks.length > 0) {
            const firstBlock = response.returnArrayOfBlocks[0];
            if (firstBlock && firstBlock.returnArray) {
              return firstBlock.returnArray.map((item: DataItemType) => ({
                name: item.ersname,
                value: item.value
              }));
            }
          }
          return [];
        }),
        catchError(err => {
          console.error('Error fetching group-abandoned-info-table data in manager:', err);
          this.widget.state!.error = err.message || 'Failed to fetch data';
          return throwError(() => err);
        })
      ).subscribe({
        next: (transformedData) => {
          this.tableData = transformedData;
          if (this.widget.state) {
            this.widget.state.data = transformedData; // Store transformed data
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
