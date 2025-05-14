import { Component, Input, OnChanges, OnInit, SimpleChanges, ChangeDetectionStrategy, ChangeDetectorRef } from '@angular/core';
import {
  SuperGroupListItemType
} from '../../../app/services/emis-soap.service'; // Adjusted path
import {CommonModule } from '@angular/common';
//import {NlatTableComponent} from "../nlat-table/nlat-table.component";
import {GsDashboardWidgetManagerService, WidgetConfig} from "../gs-dashboard-widget-manager.service";
import { BaseWidget } from 'gridstack/dist/angular';


const SUPER_GROUP_LIST_WIDGET_ID = 'super-group-list-table';


@Component({
  selector: 'gs-nla-table',
  templateUrl: './gs-nlat-table.component.html',
  styleUrls: ['./gs-nlat-table.component.scss'],
  imports: [
    CommonModule]
  //changeDetection: ChangeDetectionStrategy.OnPush
})
export class GsNlatTableComponent extends BaseWidget {
  @Input() widget!: WidgetConfig;

  tableObj: any;



  displayedColumns: string[] = ['Group ID', 'Name'];


  constructor(private cdr: ChangeDetectorRef,
              public widgetManager: GsDashboardWidgetManagerService) {
    super();
    console.log("constructor GsNlatTableComponent")
  }

  //ngOnInit() {

    //this.widgetManager.setWidgetConfig(this.widget);

    //this.widgetManager.setWidgetState(this.widget);





    //this.tableObj = this.widgetManager.getWidgetState(this.widget.id)?.data;
  //}


  /**
   * Refresh the data for this widget
   * @param widgetId The ID of the widget to refresh
   */
  refreshWidget(widgetId: string): void {
    // Call the widget manager's refresh method for this specific widget
    this.widgetManager.refreshWidget(widgetId);
  }

}
