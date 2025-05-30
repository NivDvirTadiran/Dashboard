import { Component, HostBinding, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../../../gs-dashboard-widget-manager.service';
import { GSBaseWidget } from '../../../base-widget/base-widget.component';
import { Observable, catchError, map, throwError } from 'rxjs';
import { EmisSoapService, ReturnMatrixDataTypeBG, DataItemType } from 'src/app/services/emis-soap.service';
import { GsGenericTableComponent, TableColumn, TableRowData } from '../../../gs-generic-table/gs-generic-table.component';

@Component({
  selector: 'brief-agents-widget',
  templateUrl: './brief-agents-widget.component.html',
  styleUrls: ['./brief-agents-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, GSBaseWidget, GsGenericTableComponent]
})
export class BriefAgentsWidgetComponent extends GSBaseWidget implements OnDestroy {
  @HostBinding('style.display') display = 'contents';

  tableColumns: TableColumn[] = [
    { key: "Agent Name", header: "Agent Name" },
    { key: "Agent Exten.", header: "Agent Exten." },
    { key: "State", header: "State" },
    { key: "Agent No.", header: "Agent No." },
    { key: "State Time", header: "State Time" },
    { key: "ACD Calls", header: "ACD Calls" },
    { key: "O.ACD Calls", header: "O.ACD Calls" },
    { key: "Chat Contacts", header: "Chat Contacts" },
    { key: "Email Contacts", header: "Email Contacts" },
    { key: "ACD Calls Answered", header: "ACD Calls Answered" },
    { key: "O.ACD Calls Answered", header: "O.ACD Calls Answered" },
    { key: "Chat Contacts Answered", header: "Chat Contacts Answered" },
    { key: "Email Contacts Answered", header: "Email Contacts Answered" },
    { key: "Total ACD Talk Time", header: "Total ACD Talk Time" },
    { key: "Held Calls", header: "Held Calls" },
    { key: "Longest On Held Time", header: "Longest On Held Time" },
    { key: "Total Held Calls", header: "Total Held Calls" },
    { key: "Total Held Time", header: "Total Held Time" },
    { key: "Conference", header: "Conference" },
    { key: "Time In Conference", header: "Time In Conference" },
    { key: "Total Txfr Out Calls", header: "Total Txfr Out Calls" },
    { key: "NON-ACD Calls", header: "NON-ACD Calls" },
    { key: "Total Inc NON-ACD Calls", header: "Total Inc NON-ACD Calls" },
    { key: "Total Out NON-ACD Calls", header: "Total Out NON-ACD Calls" },
    { key: "Total NON-ACD Talk Time", header: "Total NON-ACD Talk Time" },
  ];
  tableData: TableRowData[] = [];


  constructor(
    protected override widgetManager: GsDashboardWidgetManagerService
    // Remove private emisSoapService, will use super.emisSoapService from GSBaseWidget
  ) {
    super(widgetManager);
    console.log('constructor BriefAgentsWidgetComponent');
  }

  // Make fetchData public to match GSBaseWidget
  public override fetchData(): void {
    if (!this.widget || !this.widget.state) {
      // Call super.fetchData() to ensure base loading state is handled if desired,
      // or manage loading state entirely within this overridden method.
      // For now, let's assume this method fully manages its state.
      if (this.widget && this.widget.state) { // double check for safety
          this.widget.state.loading = false; // Ensure loading is false if we bail early
      }
      return;
    }

    super.fetchData(); // Handles setting loading to true

    // Retrieve selected agent IDs from widget configuration
    const selectedAgentIds = this.widget.config?.selectedAgentIds;
    console.log(`BriefAgentsWidget fetchData with selectedAgentIds:`, selectedAgentIds);

    // Pass selectedAgentIds to the service call
    // The emisSoapService is now accessed via `this.emisSoapService` (from GSBaseWidget)
    this.emisSoapService.getBriefAgents(selectedAgentIds).pipe(
        map(data => {
          console.log(`Fetched brief-agents-data with filter:`, data);
          if (data && data.returnMatrix) {
            return data.returnMatrix.map(item => {
              const row: TableRowData = {};
              if (item.returnArray && item.returnArray.length > 0 && item.returnArray[item.returnArray.length - 1]?.value) {
                const rawDataString = item.returnArray[item.returnArray.length - 1].value;
                const parsedMap = this.parseAgentDataStringToMap(rawDataString);
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
    // } // Removed redundant if (this.widget && this.widget.state)
  }

  private parseAgentDataStringToMap(rawDataString: string): Map<string, string> {
    const dataMap = new Map<string, string>();
    if (!rawDataString || typeof rawDataString !== 'string') {
      return dataMap;
    }
    const parts = rawDataString.split(',');
    parts.forEach(part => {
      const cell = part.split(';');
      if (cell.length >= 4 && cell[2] !== undefined && cell[3] !== undefined) {
        dataMap.set(cell[2], cell[3]);
      }
    });
    return dataMap;
  }

  override ngOnDestroy(): void {
    super.ngOnDestroy();
  }
}
