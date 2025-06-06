// dashboard-widget-manager.service.ts
import {Injectable, Type} from '@angular/core';
import {Subscription} from 'rxjs';
import {TableRow} from 'src/app/dashboard/dashboard.service'; // Adjusted path
import {GridstackComponent, NgGridStackWidget} from 'gridstack/dist/angular';
import {GridStack} from 'gridstack';

export interface WidgetConfig {
    id: string; // Unique identifier for the widget
    type: string; // Widget type (e.g., 'table', 'chart', etc.)
    nlat: string; // Position on the dashboard grid
    state?: WidgetState | null; // Optional title for the widget
    dataSource: string; // API endpoint or data source for the widget
    updateInterval: number; // Data refresh interval in milliseconds
    title?: string; // Optional title for the widget
    settings?: Record<string, any>; // Additional settings specific to the widget
    resizable?: boolean; // Whether the widget can be resized
    draggable?: boolean; // Whether the widget can be dragged
    config?: {
      selectedEntitiesIds?: string[];
      description?: string; // Added description to config
      [key: string]: any; // Allow other config properties
    };
};

export interface WidgetState {
    id: string; // Matches the widget ID
    data: any; // Current data for the widget
    loading: boolean; // Whether the widget is loading data
    error?: string; // Error message if data fetching fails
    lastUpdated?: number; // Timestamp of the last successful update
}

@Injectable({
    providedIn: 'root' // Or provide in a specific module if not globally available for stories
})
export class GsDashboardWidgetManagerService {
    private widgetConfigs = new Map<string, WidgetConfig>();
    private lastUpdates = new Map<string, number>();
    private widgetTypeToSelectorMap = new Map<Type<any>, string>();

    tableObj: TableRow[] = [];

    public exampleDataNew: string =
        '[{"agentName":"Agent Name","agentExten":"Agent Exten.","state":"State","agentNo":"Agent No.","stateTime":"State Time","releaseCode":"Release Code","aCDCalls":"ACD Calls","nonACDCalls":"Non ACD Calls","dNIS":"DNIS","aN":"ANI"},' +
        '{"agentName":"Agent1","agentExten":"0","state":"Logout","agentNo":"1001","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
        '{"agentName":"Agent2","agentExten":"0","state":"Logout","agentNo":"1002","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
        '{"agentName":"Agent5","agentExten":"0","state":"Logout","agentNo":"1005","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
        '{"agentName":"Agent3","agentExten":"0","state":"Logout","agentNo":"1003","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
        '{"agentName":"Agent6","agentExten":"0","state":"Logout","agentNo":"1006","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"}]';

    public subscription: Subscription;
    private _gridCompRef: GridstackComponent | null = null;

    constructor() {
        this.tableObj = JSON.parse(this.exampleDataNew);
    }


  private getGrid(): GridStack {
    return this.getGridComponent();
  }

    public registerWidgetMappings(mappings: { widgetType: Type<any>, selector: string }[]): void {
      mappings.forEach(mapping => {
        this.widgetTypeToSelectorMap.set(mapping.widgetType, mapping.selector);
      });
      console.log('Widget type to selector map populated in GsDashboardWidgetManagerService:', this.widgetTypeToSelectorMap);
    }

    public getSelectorForWidgetType(widgetType: Type<any>): string | undefined {
        return this.widgetTypeToSelectorMap.get(widgetType);
    }

    // Method to set the gridstack component reference from the parent component
    setGridComponent(gridComp: GridstackComponent): void {
        this._gridCompRef = gridComp;
        console.log('GridStack component reference set in widget manager');
    }

    // Method to get the gridstack component reference from the main widgets service
    getGridComponent(): GridStack {
        return this._gridCompRef.grid;
    }

    // Getter to access the gridstack component and its grid
    get grid() {
        return this._gridCompRef?.grid;
    }

    // Method to add widgets to the grid
    addWidgetToGrid(widget: NgGridStackWidget): void {
        if (this.grid) {
            this.grid.addWidget(widget);
        } else {
            console.error('Grid component reference not set or grid not initialized');
        }
    }

    setWidgetConfig(config: WidgetConfig): void {
        this.widgetConfigs.set(config.id, config);
    }

    getWidgetConfig(widgetId: string): WidgetConfig | undefined {
        return this.widgetConfigs.get(widgetId);
    }

    getWidgetState(widgetId: string): WidgetState | undefined {
        return this.widgetConfigs.get(widgetId).state;
    }

    getRegisteredWidgets(): WidgetConfig[] {
        return Array.from(this.widgetConfigs.values());
    }

    initGsWidget(config: WidgetConfig): WidgetConfig {
        // Initialize widget state
        config.state = {
            id: config.id,
            data: null,
            loading: false,
            error: null,
            lastUpdated: null
        };

        this.setWidgetConfig(config);

        let gridWidget: NgGridStackWidget = {
            autoPosition: true,
            w: 2,
            h: 1,
            selector: config.type,
            input: {widget: config},
            id: String(config.id)
        } as NgGridStackWidget;

        // Add the widget to grid if grid is available
        this.addWidgetToGrid(gridWidget);

        return config;

    }

    removeWidget(widgetId: string): void {
        this.widgetConfigs.delete(widgetId);
    }





  public addExampleOneViewWidget(): void {
    const ONE_VIEW_WIDGET_ID_BASE = 'one-view-widget';
    const currentGridHeight = this.getGrid().getRow() || 0;

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
      nlat: 'auto',
    };

    this.initGsWidget({
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
    const currentGridHeight = this.getGridComponent().getRow() || 0;

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
    this.initGsWidget({
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
    this.initGsWidget(newWidgetConfig);
    console.log(`Chart widget ${newWidgetConfig.id} (type: ${newWidgetConfig.type}) initialized with settings:`, chartSettings);
  }

  public addSuperGroupListWidget(): void {
    const SUPER_GROUP_LIST_WIDGET_ID = 'super-group-list-table';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.getGrid().getRow() || 0;

    newWidgetConfig = {
      type: 'super-group-list-widget',
      title: 'Super Group Status',
      dataSource: 'modernized-api-super-groups',
      updateInterval: 6000
    };

    this.initGsWidget({
      id: SUPER_GROUP_LIST_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addAgentsListWidget(): void {
    const AGENTS_LIST_WIDGET_ID = 'agents-list-table';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.getGrid().getRow() || 0;

    newWidgetConfig = {
      type: 'agents-list-widget',
      title: 'Agent Status',
      dataSource: 'AgentList',
      updateInterval: 6000
    };

    this.initGsWidget({
      id: AGENTS_LIST_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addGroupListWidget(): void {
    const GROUP_LIST_WIDGET_ID = 'group-list-table';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.getGrid().getRow() || 0;

    newWidgetConfig = {
      type: 'group-list-widget',
      title: 'Group Status',
      dataSource: 'GroupList',
      updateInterval: 6000
    };

    this.initGsWidget({
      id: GROUP_LIST_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addDnisListWidget(): void {
    const DNIS_LIST_WIDGET_ID = 'dnis-list-table';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.getGrid().getRow() || 0;

    newWidgetConfig = {
      type: 'dnis-list-widget',
      title: 'DNIS Status',
      dataSource: 'DnisList',
      updateInterval: 5000
    };

    this.initGsWidget({
      id: DNIS_LIST_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addGroupDetailFullInfoWidget(): void {
    const GROUP_DETAIL_FULL_INFO_WIDGET_ID = 'group-detail-full-info-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.getGrid().getRow() || 0;

    newWidgetConfig = {
      type: 'group-detail-full-info-widget',
      title: 'Group Detail Full Info',
      dataSource: 'GroupDetailFullInfo',
      updateInterval: 6000,
      settings: { groupId: 1 } // Example: Default to group ID 1, or prompt user
    };

    this.initGsWidget({
      id: GROUP_DETAIL_FULL_INFO_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addGroupQueueInfoWidget() {
    const GROUP_QUEUE_INFO_WIDGET_ID = 'group-queue-info-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.getGrid().getRow() || 0;

    newWidgetConfig = {
      type: 'group-queue-info-widget',
      title: 'Group Queue Info',
      dataSource: 'Group Queue Info',
      updateInterval: 6000,
      settings: { groupId: 1 } // Example: Default to group ID 1, or prompt user
    };

    this.initGsWidget({
      id: GROUP_QUEUE_INFO_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addGroupAgentsInfoWidget(): void {
    const GROUP_AGENTS_INFO_WIDGET_ID = 'group-agents-info-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.getGrid().getRow() || 0;

    newWidgetConfig = {
      type: 'group-agents-info-widget',
      title: 'Group Agents Info',
      dataSource: 'GroupAgentsInfo',
      updateInterval: 6000,
      settings: { groupId: 1 } // Example: Default to group ID 1, or prompt user
    };

    this.initGsWidget({
      id: GROUP_AGENTS_INFO_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addIvrApplicationInfoWidget(): void {
    const IVR_APPLICATION_INFO_WIDGET_ID = 'ivr-application-info-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.getGrid().getRow() || 0;

    newWidgetConfig = {
      type: 'ivr-application-info-widget',
      title: 'IVR Application Info',
      dataSource: 'IvrApplicationInfo',
      updateInterval: 6000,
      settings: { ivrId: 1 } // Example: Default to IVR ID 1, or prompt user
    };

    this.initGsWidget({
      id: IVR_APPLICATION_INFO_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addIvrPortInfoWidget(): void {
    const IVR_PORT_INFO_WIDGET_ID = 'ivr-port-info-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.getGrid().getRow() || 0;

    newWidgetConfig = {
      type: 'ivr-port-info-widget',
      title: 'IVR Port Info',
      dataSource: 'IvrPortInfo',
      updateInterval: 6000,
      settings: { portId: 1 } // Example: Default to Port ID 1, or prompt user
    };

    this.initGsWidget({
      id: IVR_PORT_INFO_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addGroupAbandonedInfoWidget(): void {
    const GROUP_ABANDONED_INFO_WIDGET_ID = 'group-abandoned-info-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.getGrid().getRow() || 0;

    newWidgetConfig = {
      type: 'group-abandoned-info-widget',
      title: 'Group Abandoned Info',
      dataSource: 'GroupAbandonedInfo',
      updateInterval: 6000,
      settings: { groupId: 1 } // Example: Default to group ID 1, or prompt user
    };

    this.initGsWidget({
      id: GROUP_ABANDONED_INFO_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addBriefAgentsWidget(): void {
    const BRIEF_AGENTS_WIDGET_ID = 'brief-agents-widget';
    let newWidgetConfig: Partial<WidgetConfig> = {};
    const currentGridHeight = this.getGrid().getRow() || 0;

    newWidgetConfig = {
      type: 'brief-agents-widget',
      title: 'Brief Agents',
      dataSource: 'BriefAgents',
      updateInterval: 6000
    };

    this.initGsWidget({
      id: BRIEF_AGENTS_WIDGET_ID + '-' + Date.now() + '-' + Math.random().toString(36).substring(2, 7),
      position: { x: 0, y: currentGridHeight }, // y is 0-based
      ...newWidgetConfig
    } as WidgetConfig);
  }

  public addBriefDnisWidget(): void {
    const BRIEF_DNIS_WIDGET_ID_BASE = 'brief-dnis-widget';
    const currentGridHeight = this.getGrid().getRow() || 0;

    const newWidgetConfig: WidgetConfig = {
      id: `${BRIEF_DNIS_WIDGET_ID_BASE}-${Date.now()}-${Math.random().toString(36).substring(2, 7)}`,
      type: 'brief-dnis-widget', // This must match the selector of BriefDnisWidgetComponent
      title: 'Brief DNIS',
      dataSource: 'BriefDnis', // This should align with what BriefDnisWidgetComponent expects
      updateInterval: 6000, // Example interval
      settings: { selectedDnisIds: [] }, // Example: Default to no specific DNIS IDs, or prompt user
      nlat: 'auto',
    };

    this.initGsWidget({
      ...newWidgetConfig,
      position: { x: 0, y: currentGridHeight },
    } as WidgetConfig);
    console.log(`Brief DNIS widget ${newWidgetConfig.id} (type: ${newWidgetConfig.type}) initialized.`);
  }
}
