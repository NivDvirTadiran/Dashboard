import { Component, Input } from '@angular/core';
import { CommonModule, JsonPipe, DatePipe } from '@angular/common';
import { GsDashboardWidgetManagerService, WidgetConfig } from '../gs-dashboard-widget-manager.service';
import { WidgetTableComponent } from '../../sb-dashboard/widget-table/widget-table.component';
import { WidgetPieChartComponent } from '../widget-pie-chart/widget-pie-chart.component';
import { WidgetPieDoughnutComponent } from '../widget-pie-doughnut/widget-pie-doughnut.component';

@Component({
  selector: 'app-widget',
  templateUrl: './widget.component.html',
  styleUrls: ['./widget.component.scss'],
  standalone: true,
  imports: [
    CommonModule,
    WidgetTableComponent,
    WidgetPieChartComponent,
    WidgetPieDoughnutComponent,
    JsonPipe,
    DatePipe
  ]
})
export class WidgetComponent {
  @Input() widget!: WidgetConfig;

  constructor(public widgetManager: GsDashboardWidgetManagerService) {}

  /**
   * Refresh the data for this widget
   * @param widgetId The ID of the widget to refresh
   */
  refreshWidget(widgetId: string): void {
    // Call the widget manager's refresh method for this specific widget
    this.widgetManager.refreshWidget(widgetId);
  }
}
