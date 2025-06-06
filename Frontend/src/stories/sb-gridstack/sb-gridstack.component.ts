import { Component, AfterViewInit, Input, OnDestroy, ViewChild, OnInit, Type } from '@angular/core';
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
import { ChartWidgetComponent } from './chart-widget/chart-widget.component';
import { ExamplePieChartWidgetComponent } from './example-pie-chart-widget/example-pie-chart-widget.component';
import { OneViewWidgetComponent } from './one-view-widget/one-view-widget.component';
import { BriefDnisWidgetComponent } from './widgets/dnis/brief-dnis-widget/brief-dnis-widget.component';

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
  @ViewChild(GridstackComponent, { static: true }) private gridComp!: GridstackComponent;

  private ids = 0; // Used for layout loading, might need adjustment for dynamic widget IDs
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
    // Component to selector type registration is still needed by Gridstack.angular
    // This allows GridStack to know which Angular component to render for a given selector string.
    const registeredWidgetTypes = [
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
      OneViewWidgetComponent,
      BriefDnisWidgetComponent
      // Ensure ALL widget components that can be added are registered here
    ];
    GridstackComponent.addComponentToSelectorType(registeredWidgetTypes);

    // Prepare mappings for GsDashboardWidgetManagerService
    // IMPORTANT: Selectors MUST match the actual @Component({selector: '...'}) values
    const widgetMappingData = [
      { widgetType: SuperGroupListWidgetComponent, selector: 'super-group-list-widget' },
      { widgetType: GsNlatTableComponent, selector: 'gs-nlat-table' },
      { widgetType: AgentsListWidgetComponent, selector: 'agents-list-widget' },
      { widgetType: GroupListWidgetComponent, selector: 'group-list-widget' },
      { widgetType: BriefAgentsWidgetComponent, selector: 'brief-agents-widget' },
      { widgetType: DnisListWidgetComponent, selector: 'dnis-list-widget' },
      { widgetType: GroupDetailFullInfoWidgetComponent, selector: 'group-detail-full-info-widget' },
      { widgetType: GroupQueueInfoWidgetComponent, selector: 'group-queue-info-widget' },
      { widgetType: GroupAbandonedInfoWidgetComponent, selector: 'group-abandoned-info-widget' },
      { widgetType: GroupAgentsInfoWidgetComponent, selector: 'group-agents-info-widget' },
      { widgetType: IvrApplicationInfoWidgetComponent, selector: 'ivr-application-info-widget' },
      { widgetType: IvrPortInfoWidgetComponent, selector: 'ivr-port-info-widget' },
      { widgetType: ChartWidgetComponent, selector: 'chart-widget' }, // Or 'app-chart-widget' etc.
      { widgetType: ExamplePieChartWidgetComponent, selector: 'example-pie-chart-widget' },
      { widgetType: OneViewWidgetComponent, selector: 'one-view-widget' }, // Or 'app-one-view-widget'
      { widgetType: BriefDnisWidgetComponent, selector: 'brief-dnis-widget' }
      // Add ALL other registered widgets here with their correct selectors
    ];

    this.widgetManager.registerWidgetMappings(widgetMappingData);
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
    // Example: { autoPosition: true, w: 2, h: 1, selector: 'brief-agents-widget', input: { widget: { id: 'initial-brief-agents', type: 'brief-agents-widget', title: 'Initial Brief Agents', ... } }, id: 'initial-brief-agents'}
  ];

  public ngOnInit(): void {
    this.loadLayout(); // Load layout on init if available
  }

  presentAddWidgetOptions(): void {
    this.showAddWidgetModal = true;
  }

  saveLayout(): void {
    const layout = this.gridComp?.grid?.save(true, true);
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
        // Ensure grid is initialized before loading layout
        if (this.gridComp?.grid) {
          this.gridComp.grid.load(layout);
          console.log('Dashboard layout loaded.');
        } else {
          // Defer loading if grid is not ready, common if static:true is not appropriate
          // or if loadLayout is called too early. For now, with static:true, it should be available.
          console.warn('Grid not ready for loading layout in loadLayout. If this persists, check ViewChild static flag and component lifecycle.');
        }

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
    if (this.gridComp) {
      this.widgetManager.setGridComponent(this.gridComp);
    }
    // this.gridComp?.grid.load(this.wItems); // Load initial items if any
    // this.registerWidgets(); // This method is likely obsolete now
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

  public onWidgetsSelected(selectedWidgetTypes: Type<any>[]): void {
    selectedWidgetTypes.forEach(widgetType => {
      const selector = this.widgetManager.getSelectorForWidgetType(widgetType);

      if (selector) {
        const newWidgetId = `${selector}-${Date.now()}-${Math.random().toString(36).substring(2, 7)}`;
        // Attempt to get a display name for the title. Fallback to selector.
        // This could be enhanced by adding a static 'widgetName' property to each component type.
        const widgetTitle = (widgetType as any).widgetName || widgetType.name.replace('WidgetComponent', '') || selector;

        const newWidgetConfig: WidgetConfig = {
          id: newWidgetId,
          type: selector, // This is the string selector for GridStack
          title: widgetTitle, // A more user-friendly title
          dataSource: '', // Determine appropriate dataSource or leave for widget to handle
          updateInterval: 60000, // Default or determine based on type
          settings: {}, // Default or determine based on type
          nlat: 'auto' // Or other appropriate default
        };

        this.widgetManager.initGsWidget(newWidgetConfig);
        console.log(`Widget added: ${newWidgetId} (Type: ${widgetType.name}, Selector: ${selector})`);
      } else {
        console.warn(`No selector found for widget type: ${widgetType.name}. Ensure it's mapped in GsDashboardWidgetManagerService and registered with GridstackComponent.`);
      }
    });
    this.showAddWidgetModal = false; // Close modal after adding widgets
  }

  public setAddWidgetModalViewState(isOpen: boolean): void {
    this.showAddWidgetModal = isOpen;
  }
}
