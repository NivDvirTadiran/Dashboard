import { Component, AfterViewInit, Input, OnDestroy, viewChild, OnInit } from '@angular/core';
import { CdkDragDrop, moveItemInArray } from '@angular/cdk/drag-drop';
import { WidgetPieDoughnutComponent } from './widget-pie-doughnut/widget-pie-doughnut.component';
import { GsDashboardWidgetManagerService, WidgetConfig } from './gs-dashboard-widget-manager.service';
import { TableRow } from 'src/app/dashboard/dashboard.service';
import { CommonModule, NgIf } from '@angular/common';
import { DashboardHeaderComponent } from './dashboard-header/dashboard-header.component';
import { AddWidgetMenuComponent } from './add-widget-menu/add-widget-menu.component';

import { GridstackComponent, GridstackModule, NgGridStackOptions, NgGridStackWidget } from 'gridstack/dist/angular';
import { GridStackWidget } from 'gridstack/dist/types';
import { GsNlatTableComponent } from './gs-nlat-table/gs-nlat-table.component';
import { SuperGroupListWidgetComponent } from './widgets/super-groups/super-group-list-widget/super-group-list-widget.component';
import { AgentsListWidgetComponent } from './widgets/agents/agents-list-widget/agents-list-widget.component';
import { GroupListWidgetComponent } from './widgets/groups/group-list-widget/group-list-widget.component';
import { BriefAgentsWidgetComponent } from './widgets/agents/brief-agents-widget/brief-agents-widget.component';
import { DnisListWidgetComponent } from './widgets/dnis/dnis-list-widget/dnis-list-widget.component';
import { GroupDetailFullInfoWidgetComponent } from './widgets/groups/group-detail-full-info-widget/group-detail-full-info-widget.component';
import { GroupQueueInfoWidgetComponent } from './widgets/groups/group-queue-info-widget/group-queue-info-widget.component';
import { GroupAbandonedInfoWidgetComponent } from './widgets/groups/group-abandoned-info-widget/group-abandoned-info-widget.component';
import { GroupAgentsInfoWidgetComponent } from './widgets/groups/group-agents-info-widget/group-agents-info-widget.component';
import { IvrApplicationInfoWidgetComponent } from './widgets/ivr-applications/ivr-application-info-widget/ivr-application-info-widget.component';
import { IvrPortInfoWidgetComponent } from './widgets/ivr-applications/ivr-port-info-widget/ivr-port-info-widget.component';
import { ChartWidgetComponent } from './widgets/chart-widget/chart-widget.component';
import { ExamplePieChartWidgetComponent } from './example-pie-chart-widget/example-pie-chart-widget.component';
import { OneViewWidgetComponent } from './one-view-widget/one-view-widget.component';

@Component({
  selector: 'sb-gridstack',
  templateUrl: './sb-gridstack.component.html',
  styleUrls: ['./sb-gridstack.component.scss'],
  standalone: true,
  imports: [
    CommonModule,
    NgIf,
    GridstackComponent,
    GridstackModule,
    DashboardHeaderComponent,
    AddWidgetMenuComponent
  ]
})
export class SbGridstackComponent implements OnInit, AfterViewInit, OnDestroy {
  private gridComp = viewChild(GridstackComponent);

  private ids = 0;
  public showAddWidgetModal = false;

  gridOptions: NgGridStackOptions = {
    margin: '0.5rem',
    marginUnit: 'rem',
    minRow: 1,
    float: true,
    column: 30,
    animate: true,
    columnOpts: { columnMax: 12, breakpoints: [{ w: 400, c: 6, layout: 'none',}, { w: 600, c: 3, layout: 'none', },], },
  };

  @Input() tableObj!: TableRow[];

  constructor(
    public widgetManager: GsDashboardWidgetManagerService
  ) {
    GridstackComponent.addComponentToSelectorType([
      SuperGroupListWidgetComponent,
      GsNlatTableComponent,
      AgentsListWidgetComponent,
      GroupListWidgetComponent,
      BriefAgentsWidgetComponent,
      DnisListWidgetComponent,
      GroupDetailFullInfoWidgetComponent,
      GroupQueueInfoWidgetComponent,
      GroupAbandonedInfoWidgetComponent,
      GroupAgentsInfoWidgetComponent,
      IvrApplicationInfoWidgetComponent,
      IvrPortInfoWidgetComponent,
      ChartWidgetComponent,
      ExamplePieChartWidgetComponent,
      OneViewWidgetComponent
    ]);
  }

  public exampleDataNew: string =
    '[{"agentName":"Agent Name","agentExten":"Agent Exten.","state":"State","agentNo":"Agent No.","stateTime":"0:00","releaseCode":"Release Code","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
    '{"agentName":"Agent1","agentExten":"0","state":"Logout","agentNo":"1001","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
    '{"agentName":"Agent2","agentExten":"0","state":"Logout","agentNo":"1002","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
    '{"agentName":"Agent5","agentExten":"0","state":"Logout","agentNo":"1005","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
    '{"agentName":"Agent3","agentExten":"0","state":"Logout","agentNo":"1003","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
    '{"agentName":"Agent6","agentExten":"0","state":"Logout","agentNo":"1006","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"}]';

  newWidgetConfig: Partial<WidgetConfig> = {};

  pieChartConfig: WidgetConfig = {
    id: 'pie-chart-widget' + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
    position: { x: 0, y: 1 },
    ...this.newWidgetConfig
  } as WidgetConfig;

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

  presentAddWidgetOptions(): void {
    this.showAddWidgetModal = true;
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

  items = ['Zero', 'One', 'Two'];
  frames: any = [WidgetPieDoughnutComponent];
  axes = ['both', 'x', 'y'];

  ngAfterViewInit(): void {
    if (this.gridComp()) {
      this.widgetManager.setGridComponent(this.gridComp());
    }
    this.gridComp()?.grid.load(this.wItems);
    this.registerWidgets();
  }

  ngOnDestroy(): void {
    // No specific cleanup needed here beyond what BaseWidget handles
  }

  refreshWidget(widgetId: string): void {
    // Delegate refresh to the widget manager, which will trigger fetchData on the specific widget
    this.widgetManager.getWidgetConfig(widgetId)?.state?.loading; // Trigger loading state
    this.widgetManager.getRegisteredWidgets().find(w => w.id === widgetId)?.state?.loading; // Ensure state is updated
    // The actual data fetching is now handled by the individual widget's fetchData method,
    // which is called by the BaseWidget's interval subscription.
    // To force an immediate refresh, we would need a public method on BaseWidget or a way to trigger its fetchData.
    // For now, rely on the interval or a direct call if a reference to the component instance is available.
  }

  refreshAllWidgets(): void {
    this.widgetManager.getRegisteredWidgets().forEach(widget => {
      console.log('refreshWidget: ' + widget.id);
      // Assuming BaseWidget's interval will handle this, or a direct call to fetchData if exposed
      // For now, this will just log, as direct refresh is handled by BaseWidget's internal interval.
      // If an immediate refresh is needed, BaseWidget would need a public refresh method.
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
    // Widgets are now added via the menu
    // You might want to load a default set of widgets here or leave it empty
    // For now, let's add a couple of default ones to have something on the screen
    this.addExamplePieChartWidget();
    this.addChartWidget();
    this.addExampleOneViewWidget();
    this.addGroupListWidget();
    this.addDnisListWidget();
    this.addGroupDetailFullInfoWidget();
    this.addGroupQueueInfoWidget();
    this.addGroupAgentsInfoWidget();
    this.addIvrApplicationInfoWidget();
    this.addBriefAgentsWidget();
  }

  public onWidgetSelected(widgetsType: string[]): void {
    widgetsType.forEach(widgetType => {
      switch (widgetType) {
        case 'examplePieChart':
          this.addExamplePieChartWidget();
          break;
        case 'chart':
          this.addChartWidget();
          break;
        case 'superGroupList':
          this.addSuperGroupListWidget();
          break;
        case 'agentsList':
          this.addAgentsListWidget();
          break;
        case 'groupList':
          this.addGroupListWidget();
          break;
        case 'dnisList':
          this.addDnisListWidget();
          break;
        case 'groupDetailFullInfo':
          this.addGroupDetailFullInfoWidget();
          break;
        case 'groupQueueInfo':
          this.addGroupQueueInfoWidget();
          break;
        case 'groupAgentsInfo':
          this.addGroupAgentsInfoWidget();
          break;
        case 'ivrApplicationInfo':
          this.addIvrApplicationInfoWidget();
          break;
        case 'ivrPortInfo':
          this.addIvrPortInfoWidget();
          break;
        case 'groupAbandonedInfo':
          this.addGroupAbandonedInfoWidget();
          break;
        case 'briefAgents':
          this.addBriefAgentsWidget();
          break;
        case 'oneViewExample': // Added new case for one-view-widget
          this.addExampleOneViewWidget();
          break;
        default:
          console.warn(`Unknown widget type: ${widgetType}`);
      }
    });
    this.showAddWidgetModal = false; // Close modal after selection
  }

  public addExampleOneViewWidget(): void {
    const ONE_VIEW_WIDGET_ID_BASE = 'one-view-widget';
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    // Define any specific settings for the OneViewWidget if needed
    // For now, we'll use the mock data defined within the OneViewWidgetComponent itself.
    // If data needed to be passed from here, it would go into 'settings' or a dedicated property.
    const oneViewSettings = {
      // Example: if we wanted to configure mainText and value from here
      // mainText: "Custom Title",
      // value: 100
    };

    const newWidgetConfig: WidgetConfig = {
      id: `${ONE_VIEW_WIDGET_ID_BASE}-${Date.now()}-${Math.random().toString(36).substring(2, 7)}`,
      type: 'app-one-view-widget', // This must match the selector of OneViewWidgetComponent
      title: 'One View Example',
      dataSource: 'localMock', // Indicates data is mocked or handled internally
      updateInterval: 0, // No external updates needed for mock data, or set as appropriate
      settings: oneViewSettings, // Pass any specific settings
      nlat: 'auto', // Or specify exact nlat coordinates if needed
      // Default w/h for one-view might be smaller, e.g., w:1, h:1 or w:2, h:1
      // These can be set here or handled by initGsWidget if it has logic for 'app-one-view-widget' type
    };

    this.widgetManager.initGsWidget({
      ...newWidgetConfig,
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      // Optionally specify w, h if different from default in initGsWidget
      // w: 1,
      // h: 1,
    } as WidgetConfig);
    console.log(`One View widget ${newWidgetConfig.id} (type: ${newWidgetConfig.type}) initialized.`);
  }

  public addExamplePieChartWidget(): void {
    const EXAMPLE_PIE_CHART_WIDGET_ID = 'example-pie-chart-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    const chartSettings = {
      // 172.28.31.91
      type: '3Dpie',
      refObjectName: 'Group:2',// refObjectName: 'agentStatusSummary' // Temporarily removed
      keys: '20_3_1_4_4,20_3_1_4_7,20_3_1_4_10,20_3_1_4_13',  // keys: 'status,count', // Temporarily removed
      title: 'Calls_Distribution_Graph', // Title for chart generation API
      size: '532x400'

      // 172.28.31.87
      //type: '3Dpie',
      //refObjectName: 'Group:4',
      //keys: '6_3_1_3_3,6_3_1_3_5,6_3_1_3_2,6_3_1_3_6,6_3_1_3_9,6_3_1_3_4,6_3_1_3_15,6_3_1_3_24',
      //title: 'Email.Contacts_Distribution_Graph',
      //size: '428x270'
    };

    newWidgetConfig = {
      id: `${EXAMPLE_PIE_CHART_WIDGET_ID}-${Date.now()}-${Math.random().toString(36).substring(2, 7)}`,
      type: 'example-pie-chart-widget',
      title: 'Example Pie Chart',
      dataSource: 'api/chart/agentStatus',
      updateInterval: 60000,
      settings: chartSettings,
      nlat: 'auto',
    };

    // initGsWidget in GsDashboardWidgetManagerService will handle autoPositioning and default w/h (currently 2,1)
    // If specific w/h are needed for chart-widget by default, initGsWidget should be modified
    // or these properties should be added to NgGridStackWidget within initGsWidget based on config.type.
    this.widgetManager.initGsWidget({
      id: EXAMPLE_PIE_CHART_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addChartWidget(): void {
    const CHART_WIDGET_ID_BASE = 'pie-chart-widget';

    const chartSettings = {
      // 172.28.31.91
      type: '3Dpie',
      refObjectName: 'Group:2',// refObjectName: 'agentStatusSummary' // Temporarily removed
      keys: '20_3_1_4_4,20_3_1_4_7,20_3_1_4_10,20_3_1_4_13',  // keys: 'status,count', // Temporarily removed
      title: 'Calls_Distribution_Graph', // Title for chart generation API
      size: '532x400'

      // 172.28.31.87
      //type: '3Dpie',
      //refObjectName: 'Group:1',
      //keys: '20_3_1_4_4,20_3_1_4_7,20_3_1_4_10,20_3_1_4_13',
      //title: 'Calls_Distribution_Graph',
      //size: '532x400'
    };

    const newWidgetConfig: WidgetConfig = {
      id: `${CHART_WIDGET_ID_BASE}-${Date.now()}-${Math.random().toString(36).substring(2, 7)}`,
      type: 'chart-widget', // This must match the selector of ChartWidgetComponent
      title: 'Agent Status Chart', // This is the title displayed on the widget header
      dataSource: 'api/chart/agentStatus', // Placeholder, actual endpoint is called by ChartWidgetComponent's service call
      updateInterval: 60000, // e.g., 60 seconds
      settings: chartSettings,
      nlat: 'auto',
    };

    // initGsWidget in GsDashboardWidgetManagerService will handle autoPositioning and default w/h (currently 2,1)
    // If specific w/h are needed for chart-widget by default, initGsWidget should be modified
    // or these properties should be added to NgGridStackWidget within initGsWidget based on config.type.
    this.widgetManager.initGsWidget(newWidgetConfig);
    console.log(`Chart widget ${newWidgetConfig.id} (type: ${newWidgetConfig.type}) initialized with settings:`, chartSettings);
  }

  public addSuperGroupListWidget(): void {
    const SUPER_GROUP_LIST_WIDGET_ID = 'super-group-list-table';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    newWidgetConfig = {
      type: 'super-group-list-widget',
      title: 'Super Group Status',
      dataSource: 'modernized-api-super-groups',
      updateInterval: 6000
    };

    this.widgetManager.initGsWidget({
      id: SUPER_GROUP_LIST_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addAgentsListWidget(): void {
    const AGENTS_LIST_WIDGET_ID = 'agents-list-table';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    newWidgetConfig = {
      type: 'agents-list-widget',
      title: 'Agent Status',
      dataSource: 'AgentList',
      updateInterval: 6000
    };

    this.widgetManager.initGsWidget({
      id: AGENTS_LIST_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addGroupListWidget(): void {
    const GROUP_LIST_WIDGET_ID = 'group-list-table';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    newWidgetConfig = {
      type: 'group-list-widget',
      title: 'Group Status',
      dataSource: 'GroupList',
      updateInterval: 6000
    };

    this.widgetManager.initGsWidget({
      id: GROUP_LIST_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addDnisListWidget(): void {
    const DNIS_LIST_WIDGET_ID = 'dnis-list-table';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    newWidgetConfig = {
      type: 'dnis-list-widget',
      title: 'DNIS Status',
      dataSource: 'DnisList',
      updateInterval: 5000
    };

    this.widgetManager.initGsWidget({
      id: DNIS_LIST_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addGroupDetailFullInfoWidget(): void {
    const GROUP_DETAIL_FULL_INFO_WIDGET_ID = 'group-detail-full-info-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    newWidgetConfig = {
      type: 'group-detail-full-info-widget',
      title: 'Group Detail Full Info',
      dataSource: 'GroupDetailFullInfo',
      updateInterval: 6000,
      settings: { groupId: 1 } // Example: Default to group ID 1, or prompt user
    };

    this.widgetManager.initGsWidget({
      id: GROUP_DETAIL_FULL_INFO_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addGroupQueueInfoWidget() {
    const GROUP_QUEUE_INFO_WIDGET_ID = 'group-queue-info-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    newWidgetConfig = {
      type: 'group-queue-info-widget',
      title: 'Group Queue Info',
      dataSource: 'Group Queue Info',
      updateInterval: 6000,
      settings: { groupId: 1 } // Example: Default to group ID 1, or prompt user
    };

    this.widgetManager.initGsWidget({
      id: GROUP_QUEUE_INFO_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addGroupAgentsInfoWidget(): void {
    const GROUP_AGENTS_INFO_WIDGET_ID = 'group-agents-info-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    newWidgetConfig = {
      type: 'group-agents-info-widget',
      title: 'Group Agents Info',
      dataSource: 'GroupAgentsInfo',
      updateInterval: 6000,
      settings: { groupId: 1 } // Example: Default to group ID 1, or prompt user
    };

    this.widgetManager.initGsWidget({
      id: GROUP_AGENTS_INFO_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addIvrApplicationInfoWidget(): void {
    const IVR_APPLICATION_INFO_WIDGET_ID = 'ivr-application-info-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    newWidgetConfig = {
      type: 'ivr-application-info-widget',
      title: 'IVR Application Info',
      dataSource: 'IvrApplicationInfo',
      updateInterval: 6000,
      settings: { ivrId: 1 } // Example: Default to IVR ID 1, or prompt user
    };

    this.widgetManager.initGsWidget({
      id: IVR_APPLICATION_INFO_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addIvrPortInfoWidget(): void {
    const IVR_PORT_INFO_WIDGET_ID = 'ivr-port-info-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    newWidgetConfig = {
      type: 'ivr-port-info-widget',
      title: 'IVR Port Info',
      dataSource: 'IvrPortInfo',
      updateInterval: 6000,
      settings: { portId: 1 } // Example: Default to Port ID 1, or prompt user
    };

    this.widgetManager.initGsWidget({
      id: IVR_PORT_INFO_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addGroupAbandonedInfoWidget(): void {
    const GROUP_ABANDONED_INFO_WIDGET_ID = 'group-abandoned-info-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    newWidgetConfig = {
      type: 'group-abandoned-info-widget',
      title: 'Group Abandoned Info',
      dataSource: 'GroupAbandonedInfo',
      updateInterval: 6000,
      settings: { groupId: 1 } // Example: Default to group ID 1, or prompt user
    };

    this.widgetManager.initGsWidget({
      id: GROUP_ABANDONED_INFO_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addBriefAgentsWidget(): void {
    const BRIEF_AGENTS_WIDGET_ID = 'brief-agents-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.gridComp()?.grid?.getRow() || 0;

    newWidgetConfig = {
      type: 'brief-agents-widget',
      title: 'Brief Agents',
      dataSource: 'BriefAgents',
      updateInterval: 6000
    };

    this.widgetManager.initGsWidget({
      id: BRIEF_AGENTS_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  private updateWidgetDisplay(widgetId: string, data: any): void {
    if (widgetId === 'table-widget') {
      this.tableObj = data;
    }
  }
}
