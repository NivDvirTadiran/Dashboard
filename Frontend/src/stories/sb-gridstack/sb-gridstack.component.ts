import { Component, AfterViewInit, Input, OnDestroy, input, viewChild, OnInit } from "@angular/core";
import { CdkDragDrop, moveItemInArray } from '@angular/cdk/drag-drop';
import { WidgetTableComponent } from "./widget-table/widget-table.component";
import { WidgetPieChartComponent } from "./widget-pie-chart/widget-pie-chart.component";
import { WidgetPieDoughnutComponent } from "./widget-pie-doughnut/widget-pie-doughnut.component";
import { GsDashboardWidgetManagerService, WidgetConfig } from "./gs-dashboard-widget-manager.service";
import {TableRow} from "src/app/dashboard/dashboard.service";
import { Subscription } from 'rxjs';
import { CommonModule, NgIf} from "@angular/common";
import { GsWidgetComponent } from "./gs-widget/gs-widget.component";

import {BaseWidget, GridstackComponent, GridstackModule, NgGridStackOptions, NgGridStackWidget, elementCB, gsCreateNgComponents, nodesCB} from 'gridstack/dist/angular';
import { GridStackWidget } from "gridstack/dist/types";
import {GsNlatTableComponent} from "./gs-nlat-table/gs-nlat-table.component";
import {SuperGroupListWidgetComponent} from "./super-group-list-widget/super-group-list-widget.component";

@Component({
  selector: "sb-gridstack",
  templateUrl: "./sb-gridstack.component.html",
  styleUrls: ["./sb-gridstack.component.scss"],
  standalone: true,
  imports: [
    CommonModule, NgIf, GridstackComponent, GridstackModule
  ]
})
export class SbGridstackComponent implements OnInit, AfterViewInit, OnDestroy {

  title = 'Angular 17 Multiple Files Upload example';

  private gridComp = viewChild(GridstackComponent);

  private ids = 0;
  public showAddWidgetModal = false; // For managing the visibility of the add widget modal

  gridOptions: NgGridStackOptions = {
    margin: 5,
    minRow: 1,
    float: true,
    column: 12,
    columnOpts: { columnMax: 12, breakpoints: [{ w: 800, c: 6, layout: 'none',}, { w: 500, c: 3, layout: 'none', },], },
  };

  @Input() tableObj!: TableRow[];

  constructor(
    public widgetManager: GsDashboardWidgetManagerService
  ) {
    GridstackComponent.addComponentToSelectorType([
      GsWidgetComponent,
      SuperGroupListWidgetComponent,
      GsNlatTableComponent
    ]);
  }

    public exampleDataNew: string =
      '[{"agentName":"Agent Name","agentExten":"Agent Exten.","state":"State","agentNo":"Agent No.","stateTime":"State Time","releaseCode":"Release Code","aCDCalls":"ACD Calls","nonACDCalls":"Non ACD Calls","dNIS":"DNIS","aN":"ANI"},' +
      '{"agentName":"Agent1","agentExten":"0","state":"Logout","agentNo":"1001","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
      '{"agentName":"Agent2","agentExten":"0","state":"Logout","agentNo":"1002","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
      '{"agentName":"Agent5","agentExten":"0","state":"Logout","agentNo":"1005","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
      '{"agentName":"Agent3","agentExten":"0","state":"Logout","agentNo":"1003","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
      '{"agentName":"Agent6","agentExten":"0","state":"Logout","agentNo":"1006","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0"aN":"0"}]';

    newWidgetConfig: Partial<WidgetConfig> = {};

  pieChartConfig: WidgetConfig = {
      id: 'pie-chart-widget' + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: 1 }, // y is 0-based
      ...this.newWidgetConfig
    } as WidgetConfig;

/*    pieChartConfig: WidgetConfig = {
      id: 'pie-chart-widget',
      position: { x: 0, y: 2 }, // y is 0-based
      type: 'pie-chart',
      title: 'Pie-Chart Type Example',
      dataSource: 'api/dashboard/pie-chart-data',
      updateInterval: 30000, // Update every 30 seconds
      resizable: true,
      draggable: true,
      ...this.newWidgetConfig
    };*/


  setTableData(tableObj: TableRow[]) {
    this.tableObj = tableObj;
  }

    public wItems: NgGridStackWidget[] = [
      //{ autoPosition: true, w: 2, h: 1, selector: 'app-decorator-based', input: { title: 'Item (decorator)'}, id: String(1)}  as NgGridStackWidget,
      //{ autoPosition: true, w: 2, h: 1, selector: 'gs-nlat-table', input: { tableObj: JSON.parse(this.exampleDataNew)}, id: String(2)}  as NgGridStackWidget,
      //{ autoPosition: true, w: 2, h: 1, selector: 'gs-widget', input: { widget: this.pieChartConfig}, id: String(8)}  as NgGridStackWidget,
    ];

  public ngOnInit(): void {
    this.loadLayout(); // Load layout on init if available
  }



  addWidgetTable() {
    this.addWidgetByType('agent-list-table');
  }

  presentAddWidgetOptions(): void {
    this.showAddWidgetModal = true;
  }

  public addSuperGroupListWidget(): void {
    const SUPER_GROUP_LIST_WIDGET_ID = 'super-group-list-table';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    newWidgetConfig = {
      type: 'super-group-list-table',
      title: 'Super Group Status',
      dataSource: 'modernized-api-super-groups',
      updateInterval: 2000
    };

    this.widgetManager.initGsWidget({
      id: SUPER_GROUP_LIST_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);


  }


  public addWidgetByType(type: string): void {
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    switch (type) {
      case 'pie-chart':
        newWidgetConfig = {
          type: 'pie-chart',
          title: 'New Pie Chart',
          dataSource: 'mock-pie-data',
          updateInterval: 60000
        };
        break;
      case 'doughnut':
        newWidgetConfig = {
          type: 'doughnut',
          title: 'New Doughnut Chart',
          dataSource: 'mock-doughnut-data',
          updateInterval: 60000,
        };
        break;
      case 'agent-list-table':
        newWidgetConfig = {
          type: 'agent-list-table',
          title: 'Agent Status Table',
          dataSource: 'AgentList',
          updateInterval: 15000,
        };
        break;
      case 'group-list-table':
        newWidgetConfig = {
          type: 'group-list-table',
          title: 'Group Status Table',
          dataSource: 'GroupList',
          updateInterval: 15000
        };
        break;
      case 'brief-agents-data':
        newWidgetConfig = {
          type: 'brief-agents-data',
          title: 'Brief Agents Data',
          dataSource: 'BriefAgents', // Or modernized API endpoint
          updateInterval: 20000
        };
        break;
      case 'super-group-list-table':
        newWidgetConfig = {
          type: 'super-group-list-table',
          title: 'Super Group Status',
          dataSource: 'modernized-api-super-groups',
          updateInterval: 30000
        };
        break;
      case 'group-abandoned-info': // This one needs a groupId
        newWidgetConfig = {
          type: 'group-abandoned-info',
          title: 'Group Abandoned Calls (Example Group 1)', // Title might be dynamic
          dataSource: 'group/{id}/abandoned-info',
          updateInterval: 45000,
          settings: { groupId: 1 } // Example: Default to group ID 1, or prompt user
        };
        break;
      default:
        console.warn('Unknown widget type:', type);
        return;
    }

    const fullConfig: WidgetConfig = {
      id: type + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig;

    this.widgetManager.registerWidget(fullConfig).subscribe(data => {
      console.log(`Widget ${fullConfig.id} of type ${type} added and received data/state:`, data);
    });
  }

  saveLayout(): void {
    const layout = this.gridComp()?.grid?.save(true, true);
    if (layout) {
      localStorage.setItem('dashboardLayout', JSON.stringify(layout));
      console.log('Dashboard layout saved.');
    }
  }

  loadLayout(): void {
    const savedLayout = localStorage.getItem('dashboardLayout');
    if (savedLayout) {
      try {
        const layout = JSON.parse(savedLayout);
        this.gridComp()?.grid?.load(layout);
        console.log('Dashboard layout loaded.');
        let maxId = 0;
        if (Array.isArray(layout)) {
          layout.forEach((node: any) => {
            const nodeId = parseInt(node.id, 10);
            if (!isNaN(nodeId) && nodeId > maxId) {
              maxId = nodeId;
            }
          });
        } else if (typeof layout === 'object' && layout !== null && Array.isArray(layout.children)) {
          layout.children.forEach((node: any) => {
            const nodeId = parseInt(node.id, 10);
            if (!isNaN(nodeId) && nodeId > maxId) {
              maxId = nodeId;
            }
          });
        }
        this.ids = maxId;
      } catch (e) {
        console.error('Error loading dashboard layout:', e);
      }
    }
  }

  private subscriptions: Map<string, Subscription> = new Map();
  widgetData: Map<string, any> = new Map();

  items = ['Zero', 'One', 'Two'];
  frames: any = [WidgetTableComponent, WidgetPieChartComponent, WidgetPieDoughnutComponent];
  axes = ['both', 'x', 'y'];

  defaultWidth = 20;
  defaultHeight = 26;

  ngAfterViewInit(): void {
    if (this.gridComp()) {
      this.widgetManager.setGridComponent(this.gridComp());
    }
    this.gridComp()?.grid.load(this.wItems)
    this.registerWidgets();
  }

  ngOnDestroy(): void {
    this.subscriptions.forEach(subscription => subscription.unsubscribe());
    ['table-widget', 'pie-chart-widget', 'doughnut-widget'].forEach(id => {
      this.widgetManager.unregisterWidget(id);
    });
  }

  refreshWidget(widgetId: string): void {
    this.widgetManager.refreshWidget(widgetId);
  }

  refreshAllWidgets(): void {
    this.widgetManager.getRegisteredWidgets().forEach(widget => {
      console.log("refreshWidget: " + widget.id);
    });
    this.widgetManager.getRegisteredWidgets().forEach(widget => {
      this.refreshWidget(widget.id);
    });
  }

  getTableHeaders(widgetId: string): string[] {
    const widgetState = this.widgetManager.getWidgetState(widgetId);
    if (widgetState?.data && Array.isArray(widgetState.data) && widgetState.data.length > 0) {
      return Object.keys(widgetState.data[0]).map(key => key.charAt(0).toUpperCase() + key.slice(1));
    }
    return [];
  }

  private registerWidgets(): void {
    this.addWidgetByType('pie-chart');
    this.addWidgetByType('doughnut');
    this.addWidgetByType('agent-list-table');
    this.addWidgetByType('group-list-table');
    this.addWidgetByType('brief-agents-data');
    this.addSuperGroupListWidget();
    // For group-abandoned-info, it might be better to add it manually via UI
    // as it requires a specific group ID. Or add a default one:
    this.addWidgetByType('group-abandoned-info');
  }


  private subscribeToWidget(config: WidgetConfig): void {
    const subscription = this.widgetManager.registerWidget(config)
      .subscribe(data => {
        if (data) {
          this.widgetData.set(config.id, data);
          this.updateWidgetDisplay(config.id, data);
        }
      });

    this.subscriptions.set(config.id, subscription);
  }

  private updateWidgetDisplay(widgetId: string, data: any): void {
    if (widgetId === 'table-widget') {
      this.tableObj = data;
    }
  }

  drop(event: CdkDragDrop<string[]>): void {
    moveItemInArray(this.frames, event.previousIndex, event.currentIndex);
    this.updateWidgetPositions();
  }

  updateWidgetPositions(): void {}

}
