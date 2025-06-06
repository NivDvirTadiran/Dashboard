import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { catchError, map, throwError } from 'rxjs';
import { EmisSoapService, AgentsListDataItemType } from 'src/app/services/emis-soap.service';
import { GsGenericTableComponent, TableColumn, TableRowData } from '../../../generic-widget-content/gs-generic-table/gs-generic-table.component';

@Component({
  selector: 'agents-list-widget',
  templateUrl: './agents-list-widget.component.html',
  styleUrls: ['./agents-list-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, GSBaseWidget, GsGenericTableComponent]
})
export class AgentsListWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  tableColumns: TableColumn[] = [
    { key: 'agentName', header: 'שם משתמש' },
    { key: 'agentNumber', header: 'מספר שלוחה' }, // Added agentNumber as per interface
    { key: 'agentId', header: 'מזהה נציג' } // Changed header for agentId
    // Other columns like 'סטטוס', 'נפתחו', etc. are removed as they are not in AgentsListDataItemType
  ];
  tableData: TableRowData[] = [];

  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService,
    public override emisSoapService: EmisSoapService
  ) {
    super(widgetManager);
    console.log('constructor AgentsListWidgetComponent');
  }

  public override fetchData(): void {
    if (this.widget && this.widget.state) {
      this.widget.state.loading = true;
      this.widget.state.error = undefined;

      this.emisSoapService.getAgentsList().pipe(
        map((data: AgentsListDataItemType[]) => {
          console.log(`Fetched agents-list-table:`, data);
          // Transform data to TableRowData[] based on available fields
          return (data || []).map(agent => ({
            agentName: agent.agentName,
            agentNumber: agent.agentNumber,
            agentId: agent.agentId
            // Other fields are not available in AgentsListDataItemType
          }));
        }),
        catchError((err) => {
          console.error('Error fetching agents-list-table data in manager:', err);
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
