import { Component, EventEmitter, Output, OnInit, Type } from '@angular/core';
import { CommonModule } from '@angular/common';
import { CategoryData, WidgetCategoryComponent } from './widget-category/widget-category.component';
import { SearchBarComponent } from './search-bar/search-bar.component';

// Import all necessary widget components
import { BriefAgentsWidgetComponent } from '../widgets/agents/brief-agents-widget/brief-agents-widget.component';
//import { ExamplePieChartWidgetComponent } from '../example-pie-chart-widget/example-pie-chart-widget.component';
//import { ChartWidgetComponent } from '../chart-widget/chart-widget.component';
import { BriefDnisWidgetComponent } from '../widgets/dnis/brief-dnis-widget/brief-dnis-widget.component';
// ... import ALL other specific widget components used in categories ...
// For Group widgets, import specific components if they exist, or a generic one
import { GroupAbandonedInfoWidgetComponent } from '../widgets/groups/group-abandoned-info-widget/group-abandoned-info-widget.component';
import { SuperGroupListWidgetComponent } from '../widgets/super-groups/super-group-list-widget/super-group-list-widget.component';
import {AgentsListWidgetComponent} from "../widgets/agents/agents-list-widget/agents-list-widget.component";
import {GroupListWidgetComponent} from "../widgets/groups/group-list-widget/group-list-widget.component";
import {DnisListWidgetComponent} from "../widgets/dnis/dnis-list-widget/dnis-list-widget.component";
import { GroupQueueInfoWidgetComponent } from '../widgets/groups/group-queue-info-widget/group-queue-info-widget.component';
import {
  GroupAgentsInfoWidgetComponent
} from "../widgets/groups/group-agents-info-widget/group-agents-info-widget.component";
import {
  IvrApplicationInfoWidgetComponent
} from "../widgets/ivr-applications/ivr-application-info-widget/ivr-application-info-widget.component";
import {
  IvrPortInfoWidgetComponent
} from "../widgets/ivr-applications/ivr-port-info-widget/ivr-port-info-widget.component";
import { IvrBriefReportWidgetComponent } from '../widgets/ivr/ivr-brief-report-widget/ivr-brief-report-widget.component'; // Added
import { IvrAcdDistributionGraphWidgetComponent } from '../widgets/ivr/ivr-acd-distribution-graph-widget/ivr-acd-distribution-graph-widget.component'; // Added
import { IvrActiveAcdGraphWidgetComponent } from '../widgets/ivr/ivr-active-acd-graph-widget/ivr-active-acd-graph-widget.component';
import { IvrActiveOacdGraphWidgetComponent } from '../widgets/ivr/ivr-active-oacd-graph-widget/ivr-active-oacd-graph-widget.component';
import { IvrOacdDistributionGraphWidgetComponent } from '../widgets/ivr/ivr-oacd-distribution-graph-widget/ivr-oacd-distribution-graph-widget.component';
//import { OneViewWidgetComponent } from '../one-view-widget/one-view-widget.component';
import {GsDashboardWidgetManagerService, WidgetConfig} from "../gs-dashboard-widget-manager.service";
import { GridStack } from 'gridstack';
import {
  GroupDetailFullInfoWidgetComponent
} from "../widgets/groups/group-detail-full-info-widget/group-detail-full-info-widget.component";
import {
  EmailContactsDistributionGraphWidgetComponent
} from "../widgets/groups/email-contacts-distribution-graph-widget/email-contacts-distribution-graph-widget.component";
// ... and so on for all group widgets ...
// Placeholder for group widgets if specific components are not yet defined/imported
// You'll need to create this or map to actual components
class PlaceholderGroupWidgetComponent {}
class PlaceholderIvrWidgetComponent {}
class PlaceholderSuperGroupWidgetComponent {}

@Component({
  selector: 'app-add-widget-menu',
  templateUrl: './add-widget-menu.component.html',
  styleUrls: ['./add-widget-menu.component.scss'],
  standalone: true,
  imports: [CommonModule, SearchBarComponent, WidgetCategoryComponent]
})
export class AddWidgetMenuComponent implements OnInit {
  @Output() widgetsSelected = new EventEmitter<Type<any>[]>();
  @Output() setAddWidgetModalViewState = new EventEmitter<boolean>();
  @Output() closeMenu = new EventEmitter<void>();

  selectedWidgetTypes: Set<Type<any>> = new Set();
  searchQuery: string = '';

  categories: CategoryData[] = [
    {
      id: 'agents',
      title: 'Agents',
      icon: '',
      isOpen: true,
      selectedCount: 0,
      totalCount: 0,
      widgets: [
        { id: BriefAgentsWidgetComponent, icon: '📅', contentType: 'table', title: 'Brief Agents' },
        // { id: ExamplePieChartWidgetComponent, icon: '', title: 'Example Pie Chart' },
        // { id: ChartWidgetComponent, icon: '', title: 'Chart Widget (API)' }
      ]
    },
    {
      id: 'dnis',
      title: 'DNIS',
      icon: '',
      isOpen: false,
      selectedCount: 0,
      totalCount: 0,
      widgets: [
        { id: BriefDnisWidgetComponent, icon: '📅', contentType: 'table', title: 'Brief Voice DNIS' },
      ]
    },
    {
      id: 'groups',
      title: 'Groups',
      icon: '👥',
      isOpen: false,
      selectedCount: 0,
      totalCount: 0,
      widgets: [
        // IMPORTANT: Replace PlaceholderGroupWidgetComponent with actual component types
        { id: GroupAbandonedInfoWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Abandoned Calls' },                                 // Abandoned Calls
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Above T.ASA' },                                           // Above T.ASA   one-view
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Accepted Calls' },                                     // Accepted Calls   one-view
        { id: PlaceholderGroupWidgetComponent, icon: '📈', contentType: 'graph', title: 'Agents Distribution Graph' },               // Agents Distribution Graph
        { id: PlaceholderGroupWidgetComponent, icon: '📈', contentType: 'graph', title: 'Agents MultiDistribution Graph', showPreview: false },     // Agents MultiDistribution Graph
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Agents Logged In' },                                  // Agents Logged In    one-view
        { id: PlaceholderGroupWidgetComponent, icon: '📅', contentType: 'table', title: 'Agents Report' },                                        // Agents Report    table
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', title: 'Answered Calls' },                                      // Answered Calls    one-view
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', title: 'Answered Calls Percentage' },                 // Answered Calls Percentage   one-view
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', title: 'Average Q Time' },                                       // Average Q Time   one-view
        { id: PlaceholderGroupWidgetComponent, icon: '📈', contentType: 'graph', title: 'Calls Distribution Graph' },                   // Calls Distribution Graph
        { id: PlaceholderGroupWidgetComponent, icon: '📈', contentType: 'graph', title: 'Calls Overview Graph' },                           // Calls Overview Graph
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Daily Accepted Report' },                         // Daily Accepted Report   one-view
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Daily Abandoned Report' },                       // Daily Abandoned Report   one-view
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Daily Answered Report' },                         // Daily Answered Report   one-view
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Daily Answered Calls Percentage' },     // Daily Answered Calls Percentage   one-view
        { id: PlaceholderGroupWidgetComponent, icon: '📅', contentType: 'table', title: 'Daily Brief Report' },                               // Daily Brief Report
        { id: PlaceholderGroupWidgetComponent, icon: '📋', contentType: 'report', title: 'Daily Detailed Report' },                         // Daily Detailed Report
        { id: PlaceholderGroupWidgetComponent, icon: '📋', contentType: 'report', title: 'Detailed Report' },                                     // Detailed Report
        { id: PlaceholderGroupWidgetComponent, icon: '📅', contentType: 'table', title: 'Email Brief Report' },                               // Email Brief Report   table
        { id: EmailContactsDistributionGraphWidgetComponent, icon: '📈', contentType: 'graph', title: 'Email Contacts Distribution Graph' }, // Email Contacts Distribution Graph
        { id: PlaceholderGroupWidgetComponent, icon: '📅', contentType: 'table', title: 'Email Daily Brief Report' },                   // Email Daily Brief Report   table
        { id: PlaceholderGroupWidgetComponent, icon: '📋', title: 'Email Daily Detailed Report' },             // Email Daily Detailed Report     ?
        { id: PlaceholderGroupWidgetComponent, icon: '📋', title: 'Email Detailed Report' },                         // Email Detailed Report      ?
        { id: PlaceholderGroupWidgetComponent, icon: '📋', contentType: 'report', title: 'Email Overflow-Interflow Report' },     // Email Overflow-Interflow Report    !?
        { id: PlaceholderGroupWidgetComponent, icon: '📈', contentType: 'graph', title: 'Email Queued Contacts Graph' },             // Email Queued Contacts Graph
        { id: PlaceholderGroupWidgetComponent, icon: '📈', contentType: 'graph', title: 'Email STI Graph' },                                     // Email STI Graph
        { id: PlaceholderGroupWidgetComponent, icon: '📅', title: 'Hourly Report' },                                         // Hourly Report
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Queued Calls' },                                           // Queued Calls    one-view
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Interflow Out' },                                         // Interflow Out    one-view
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Longest Call In Queue' },                         // Longest Call In Queue    one-view
        { id: PlaceholderGroupWidgetComponent, icon: '📈', contentType: 'graph', title: 'OACD STI Graph' },                                       // OACD STI Graph
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Outbound Longest Pending Call' },                 // Outbound Longest Pend    one-view
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Outbound Pending Calls' },                       // Outbound Pending Calls    one-view
        { id: PlaceholderGroupWidgetComponent, icon: '📋', contentType: 'report', title: 'Overflow-Interflow Report' },                 // Overflow-Interflow Report
        { id: PlaceholderGroupWidgetComponent, icon: '~', title: 'Overview' },                                                   // Overview
        { id: PlaceholderGroupWidgetComponent, icon: '📈', contentType: 'graph', title: 'Queue Status Graph' },                               // Queue Status Graph
        { id: PlaceholderGroupWidgetComponent, icon: '📈', contentType: 'graph', title: 'Queued Calls Graph' },                               // Queued Calls Graph
        { id: PlaceholderGroupWidgetComponent, icon: '📈', contentType: 'graph', title: 'Queued OACD Calls Graph' },                     // Queued OACD Calls Graph
        { id: PlaceholderGroupWidgetComponent, icon: '📈', contentType: 'graph', title: 'STI Graph' },                                                 // STI Graph
        { id: PlaceholderGroupWidgetComponent, icon: '📅', contentType: 'table', title: 'Status Brief Report' },                             // Status Brief Report
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'TSF Percentage' },                                       // TSF Percentage
        { id: PlaceholderGroupWidgetComponent, icon: '◻️', contentType: 'one-view', title: 'Waiting Calls In Queue' }                        // Waiting Calls In Queue    one-view
      ]
    },
    {
      id: 'ivr-applications',
      title: 'IVR Applications',
      icon: '',
      isOpen: false,
      selectedCount: 0,
      totalCount: 0,
      widgets: [
        { id: IvrAcdDistributionGraphWidgetComponent, icon: '🔵', contentType: 'pie', title: 'IVR ACD Distribution Graph' },
        { id: IvrActiveAcdGraphWidgetComponent, icon: '📈', contentType: 'graph', title: 'IVR Active ACD Graph' },
        { id: IvrActiveOacdGraphWidgetComponent, icon: '📈', contentType: 'graph', title: 'IVR Active OACD Graph' },
        { id: IvrBriefReportWidgetComponent, icon: '📅', contentType: 'table', title: 'IVR Brief Report' },
        { id: IvrOacdDistributionGraphWidgetComponent, icon: '🔵', contentType: 'pie', title: 'IVR OACD Distribution Graph' }
      ] // { id: PlaceholderIvrWidgetComponent, icon: '📋', contentType: 'report', title: 'IVR Detailed Report' }, // This one remains a placeholder as per user's initial file

    },
    {
      id: 'super-groups',
      title: 'Super Groups',
      icon: '',
      isOpen: false,
      selectedCount: 0,
      totalCount: 0,
      widgets: [
        // IMPORTANT: Replace PlaceholderSuperGroupWidgetComponent with actual component types
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Abandoned Calls' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Above T.ASA'},
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Accepted Calls' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Agents in ACD' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Agents in IDLE' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Agents_Loggedin'},
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Agents in Non ACD' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Agents in Release' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Agents in Split' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Answered Calls' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Brief Report' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Daily Abandoned Report' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Daily Accepted Report' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Daily Answered Report' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Daily Brief Report' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Queued Calls' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Longest Call In Queue' },
        { id: PlaceholderSuperGroupWidgetComponent, icon: '', title: 'Outbound Longest Pend' },
      ]
    }
  ];

  constructor(public widgetManager: GsDashboardWidgetManagerService) {}


  private getGrid(): GridStack {
    return this.widgetManager.getGridComponent();
  }

  ngOnInit(): void {
    this.categories.forEach(category => category.totalCount = category.widgets.length);
  }

  onSearchChange(query: string): void {
    this.searchQuery = query;
  }

  onCategoryToggle(categoryId: string): void {
    const category = this.categories.find(c => c.id === categoryId);
    if (category) {
      category.isOpen = !category.isOpen;
    }
  }

  onWidgetSelectionChange(event: {widgetId: Type<any>, selected: boolean}): void {
    if (event.selected) {
      this.selectedWidgetTypes.add(event.widgetId);
    } else {
      this.selectedWidgetTypes.delete(event.widgetId);
    }
    this.updateCategoryCounts(event.widgetId, event.selected);
  }

  onWidgetThumbnailClick(_widgetId: Type<any>): void {
    // For now, clicking thumbnail does nothing if checkboxes are the main selection method.
  }

  onWidgetPreviewClick(widgetId: Type<any>): void {
    console.log('Preview clicked for widget type:', widgetId.name);
  }

  updateCategoryCounts(widgetType: Type<any>, isSelected: boolean): void {
    for (const category of this.categories) {
      const widgetExistsInCategory = category.widgets.some(widget => widget.id === widgetType);
      if (widgetExistsInCategory) {
        if (isSelected) {
          category.selectedCount++;
        } else {
          category.selectedCount--;
        }
        break;
      }
    }
  }

  resetSelections(): void {
    this.selectedWidgetTypes.clear();
    this.categories.forEach(category => category.selectedCount = 0);
  }

  onAddSelectedWidgets(): void {
    //this.widgetsSelected.emit(Array.from(this.selectedWidgetTypes));
    this.selectedWidgetTypes.forEach(widgetType => {
      switch (widgetType) {
        case EmailContactsDistributionGraphWidgetComponent:
          this.widgetManager.addEmailContactsDistributionGraphWidget();
        break;
       // case ChartWidgetComponent:
       //    this.widgetManager.addChartWidget();
       // break;
        case SuperGroupListWidgetComponent:
          this.widgetManager.addSuperGroupListWidget();
          break;
        case AgentsListWidgetComponent:
          this.widgetManager.addAgentsListWidget();
          break;
        case GroupListWidgetComponent:
          this.widgetManager.addGroupListWidget();
          break;
        case DnisListWidgetComponent:
          this.widgetManager.addDnisListWidget();
          break;
        case GroupDetailFullInfoWidgetComponent:
          this.widgetManager.addGroupDetailFullInfoWidget();
          break;
        case GroupQueueInfoWidgetComponent:
          this.widgetManager.addGroupQueueInfoWidget();
          break;
        case GroupAgentsInfoWidgetComponent:
          this.widgetManager.addGroupAgentsInfoWidget();
          break;
        case IvrApplicationInfoWidgetComponent:
          this.widgetManager.addIvrApplicationInfoWidget();
          break;
        case IvrPortInfoWidgetComponent:
          this.widgetManager.addIvrPortInfoWidget();
          break;
        case GroupAbandonedInfoWidgetComponent:
          this.widgetManager.addGroupAbandonedInfoWidget();
          break;
        case BriefAgentsWidgetComponent:
          this.widgetManager.addBriefAgentsWidget();
          break;
        //case OneViewWidgetComponent: // Added new case for one-view-widget
        //  this.widgetManager.addExampleOneViewWidget();
        //  break;
        case BriefDnisWidgetComponent:
          this.widgetManager.addBriefDnisWidget();
          break;
        case IvrBriefReportWidgetComponent: // Added
          this.widgetManager.addIvrBriefReportWidget(); // Added
          break; // Added
        case IvrAcdDistributionGraphWidgetComponent: // Added
          this.widgetManager.addIvrAcdDistributionGraphWidget(); // Added
          break; // Added
        case IvrActiveAcdGraphWidgetComponent:
          this.widgetManager.addIvrActiveAcdGraphWidget();
          break;
        case IvrActiveOacdGraphWidgetComponent:
          this.widgetManager.addIvrActiveOacdGraphWidget();
          break;
        case IvrOacdDistributionGraphWidgetComponent:
          this.widgetManager.addIvrOacdDistributionGraphWidget();
          break;
        default:
          console.warn(`Unknown widget type: ${widgetType}`);
      }
    });
    this.setAddWidgetModalViewState.emit(false); // Close modal after selection
    this.resetSelections();
  }

  get addComponentsButtonText(): string {
    const count = this.selectedWidgetTypes.size;
    return count > 0 ? `Add ${count} Component${count > 1 ? 's' : ''}` : 'Add Components';
  }

  get filteredCategories(): CategoryData[] {
    if (!this.searchQuery) {
      return this.categories;
    }
    const lowerCaseQuery = this.searchQuery.toLowerCase();
    return this.categories.map(category => ({
      ...category,
      widgets: category.widgets.filter(widget =>
        widget.title.toLowerCase().includes(lowerCaseQuery)
      )
    })).filter(category => category.widgets.length > 0);
  }


}
