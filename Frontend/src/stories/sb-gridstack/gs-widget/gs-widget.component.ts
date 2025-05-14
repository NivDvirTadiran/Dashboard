import { Component, Input } from '@angular/core';
import { CommonModule, JsonPipe, DatePipe } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../gs-dashboard-widget-manager.service';
import { WidgetTableComponent } from '../widget-table/widget-table.component';
import { WidgetPieChartComponent } from '../widget-pie-chart/widget-pie-chart.component';
import { WidgetPieDoughnutComponent } from '../widget-pie-doughnut/widget-pie-doughnut.component';
import {BaseWidget} from 'gridstack/dist/angular';
import {WidgetTable2Component} from "../widget-table-2/widget-table2.component";
import {NlatTableComponent} from "../nlat-table/nlat-table.component";


@Component({
  selector: 'gs-widget',
  templateUrl: './gs-widget.component.html',
  styleUrls: ['./gs-widget.component.scss'],
  standalone: true,
  imports: [
    CommonModule,
    WidgetTableComponent,
    WidgetPieChartComponent,
    WidgetPieDoughnutComponent,
    NlatTableComponent
  ]
})
export class GsWidgetComponent extends BaseWidget {
  @Input() widget!: WidgetConfig;

  constructor(public widgetManager: GsDashboardWidgetManagerService) {
    super();

    console.log("constructor GsWidgetComponent")
  }

  /**
   * Refresh the data for this widget
   * @param widgetId The ID of the widget to refresh
   */
  refreshWidget(widgetId: string): void {
    // Call the widget manager's refresh method for this specific widget
    this.widgetManager.refreshWidget(widgetId);
  }
}
