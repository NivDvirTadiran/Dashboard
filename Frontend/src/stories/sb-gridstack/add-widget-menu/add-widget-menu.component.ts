import { Component, EventEmitter, Output } from '@angular/core';
import { CommonModule } from '@angular/common';
import { CategoryData, WidgetCategoryComponent } from './widget-category/widget-category.component';
import { SearchBarComponent } from './search-bar/search-bar.component';

@Component({
  selector: 'app-add-widget-menu',
  templateUrl: './add-widget-menu.component.html',
  styleUrls: ['./add-widget-menu.component.scss'],
  standalone: true,
  imports: [CommonModule, SearchBarComponent, WidgetCategoryComponent]
})
export class AddWidgetMenuComponent {
  @Output() widgetsSelected = new EventEmitter<string[]>(); // Changed to emit array
  @Output() closeMenu = new EventEmitter<void>();

  selectedWidgetTypes: Set<string> = new Set();
  searchQuery: string = '';

  categories: CategoryData[] = [
    {
      id: 'agents',                                                                                           //Brief Report
      title: 'Agents',                                                                                        // Needing Help
      icon: '📊',
      isOpen: true,
      selectedCount: 0,
      totalCount: 3,
      widgets: [
        { id: 'briefAgents', icon: '📅', title: 'Brief Agents' },
       /* { id: 'examplePieChart', icon: '🥧', title: 'Example Pie Chart' },
        { id: 'chart', icon: '📈', title: 'Chart Widget (API)' }*/
      ]
    },
    {
      id: 'dnis',                                                                                              //Brief Report   table
      title: 'DNIS',                                                                                           // Email Brief Report
      icon: '📋',
      isOpen: false,
      selectedCount: 0,
      totalCount: 4,
      widgets: [
        { id: 'briefDnis', icon: '👥', title: 'Brief Voice DNIS' },
        /*{ id: 'agentsList', icon: '🧑‍💼', title: 'Agents List' },
        { id: 'groupList', icon: '🏢', title: 'Group List' },
        { id: 'dnisList', icon: '📞', title: 'DNIS List' }*/
      ]
    },
    {
      id: 'groups',
      title: 'Groups',                      // Sample  one-view
      icon: 'ℹ️',
      isOpen: false,
      selectedCount: 0,
      totalCount: 6,
      widgets: [

        { id: 'groupAbandonedCalls', icon: '📄', title: 'Abandoned Calls' },                                  // Abandoned Calls
        { id: 'groupAboveT.ASA', icon: '◻️', title: 'Above T.ASA' },                                          // Above T.ASA   one-view
        { id: 'groupAcceptedCalls', icon: '◻️', title: 'Accepted Calls' },                                    // Accepted Calls   one-view
        { id: 'groupAgentsDistributionGraph', icon: '📈', title: 'Agents Distribution Graph' },               // Agents Distribution Graph
        { id: 'groupAgentsMultiDistributionGraph', icon: '📈', title: 'Agents MultiDistribution Graph', showPreview: false },     // Agents MultiDistribution Graph
        { id: 'groupAgentsLoggedIn', icon: '👥', title: 'Agents Logged In' },                                  // Agents Logged In   one-view
        { id: 'groupAgentsReport', icon: '📅', title: 'Agents Report' },                                       // Agents Report   table
        { id: 'groupAnsweredCalls', icon: '◻️', title: 'Answered Calls' },                                     // Answered Calls   one-view
        { id: 'groupAnsweredCallsPercentage', icon: '◻️', title: 'Answered Calls Percentage' },                // Answered Calls Percentage  one-view
        { id: 'groupAverageQTime', icon: '◻️', title: 'Average Q Time' },                                      // Average Q Time   one-view
        { id: 'groupCallsDistributionGraph', icon: '📈', title: 'Calls Distribution Graph' },                  // Calls Distribution Graph
        { id: 'groupCallsOverviewGraph', icon: '📈', title: 'Calls Overview Graph' },                          // Calls Overview Graph
        { id: 'groupDailyAcceptedReport', icon: '📋', title: 'Daily Accepted Report' },                        // Daily Accepted Report   one-view
        { id: 'groupDailyAbandonedReport', icon: '📋', title: 'Daily Abandoned Report' },                      // Daily Abandoned Report  one-view
        { id: 'groupDailyAnsweredReport', icon: '📋', title: 'Daily Answered Report' },                        // Daily Answered Report   one-view
        { id: 'groupDailyAnsweredCallsPercentage', icon: '📋', title: 'Daily Answered Calls Percentage' },     // Daily Answered Calls Percentage  one-view
        { id: 'groupDailyBriefReport', icon: '📋', title: 'Daily Brief Report' },                              // Daily Brief Report}// Daily Brief Report
        { id: 'groupDailyDetailedReport', icon: '📋', title: 'Daily Detailed Report' },                        // Daily Detailed Report
        { id: 'groupDitailedReport', icon: '📋', title: 'Detailed Report' },                                   // Detailed Report
        { id: 'groupEmailBriefReport', icon: '📋', title: 'Email Brief Report' },                              // Email Brief Report   table
        { id: 'groupEmailContactsDistributionGraph', icon: '📈', title: 'Email Contacts Distribution Graph' }, // Email Contacts Distribution Graph
        { id: 'groupEmailDailyBriefReport', icon: '📋', title: 'Email Daily Brief Report' },                   // Email Daily Brief Report   table
        { id: 'groupEmailDailyDetailedReport', icon: '📋', title: 'Email Daily Detailed Report' },             // Email Daily Detailed Report     ?
        { id: 'groupEmailDetailedReport', icon: '📋', title: 'Email Detailed Report' },                        // Email Detailed Report      ?
        { id: 'groupEmailOverflowInterflowReport', icon: '📋', title: 'Email Overflow-Interflow Report' },     // Email Overflow-Interflow Report    !?
        { id: 'groupEmailQueuedContactsGraph', icon: '📈', title: 'Email Queued Contacts Graph' },             // Email Queued Contacts Graph
        { id: 'groupEmailSTIGraph', icon: '📊', title: 'Email STI Graph' },                                    // Email STI Graph
        { id: 'groupHourlyReport', icon: '📅', title: 'Hourly Report' },                                       // Hourly Report
        { id: 'groupQueuedCalls', icon: '📊', title: 'Queued Calls' },                                         // Queued Calls   one-view
        { id: 'groupInterflowOut', icon: '📊', title: 'Interflow Out' },                                       // Interflow Out  one-view
        { id: 'groupLongestCallInQueue', icon: '📊', title: 'Longest Call In Queue' },                         // Longest Call In Queue one-view
        { id: 'groupOACDSTIGraph', icon: '📊', title: 'OACD STI Graph' },                                      // OACD STI Graph
        { id: 'groupOutboundLongestPendingCall', icon: '📊', title: 'Outbound Longest Pending Call' },         // Outbound Longest Pend   one-view
        { id: 'groupOutboundPendingCalls', icon: '📊', title: 'Outbound Pending Calls' },                      // Outbound Pending Calls  one-view
        { id: 'groupOverflowInterflowReport', icon: '📊', title: 'Overflow-Interflow Report' },                // Overflow-Interflow Report
        { id: 'groupOverview', icon: '📊', title: 'Overview' },                                                // Overview
        { id: 'groupQueueStatusGraph', icon: '📊', title: 'Queue Status Graph' },                              // Queue Status Graph
        { id: 'groupQueuedCallsGraph', icon: '📈', title: 'Queued Calls Graph' },                              // Queued Calls Graph
        { id: 'groupQueuedOACDCallsGraph', icon: '📈', title: 'Queued OACD Calls Graph' },                     // Queued OACD Calls Graph
        { id: 'groupSTIGraph', icon: '📊', title: 'STI Graph' },                                               // STI Graph
        { id: 'groupStatusBriefReport', icon: '📊', title: 'Status Brief Report' },                            // Status Brief Report
        { id: 'groupTSFPercentage', icon: '📊', title: 'TSF Percentage' },                                     // TSF Percentage
        { id: 'groupWaitCallsInQueue', icon: '📊', title: 'Waiting Calls In Queue' }                           // Waiting Calls In Queue  one-view
      ]
    },
    {
      id: 'ivr-applications',
      title: 'IVR Applications',
      icon: 'ℹ️',
      isOpen: false,
      selectedCount: 0,
      totalCount: 7,
      widgets: [
        { id: 'ivrACDDistributionGraph', icon: '📊', title: 'IVR ACD Distribution Graph' },          // ACD Distribution Graph
        { id: 'ivrActiveACDGraph', icon: '📊', title: 'IVR Active ACD Graph' },                      // Active ACD Graph
        { id: 'ivrActiveOACDGraph', icon: '📊', title: 'IVR Active OACD Graph' },                    // Active OACD Graph
        { id: 'ivrBriefReport', icon: '📊', title: 'IVR Brief Report' },                             // Brief Report
        { id: 'ivrDetailedReport', icon: '📊', title: 'IVR Detailed Report' },                       // Detailed Report
        { id: 'ivrOACDDistributionGraph', icon: '📊', title: 'IVR OACD Distribution Graph' }         // OACD Distribution Graph
      ]
    },
    {
      id: 'super-groups',
      title: 'Super Groups',
      icon: '📋',
      isOpen: false,
      selectedCount: 0,
      totalCount: 4,
      widgets: [
        { id: 'superGroupAbandonedCalls', icon: '📄', title: 'Abandoned Calls' },                           // Abandoned Calls
        { id: 'superGroupAboveT.ASA', icon: '◻️', title: 'Above T.ASA'},                                    // Above T.ASA
        { id: 'superGroupAcceptedCalls', icon: '📄', title: 'Accepted Calls' },                             // Accepted Calls
        { id: 'superGroupAgentsInACD', icon: '👤', title: 'Agents in ACD' },                                // Agents in ACD
        { id: 'superGroupAgentsInIDLE', icon: '👤', title: 'Agents in IDLE' },                              // Agents in IDLE
        { id: 'superGroupAgents_Loggedin', icon: '👤', title: 'Agents_Loggedin'},                           // Agents_Loggedin
        { id: 'superGroupAgentsInNonACD', icon: '👤', title: 'Agents in Non ACD' },                          // Agents in Non ACD
        { id: 'superGroupAgentsInRelease', icon: '👤', title: 'Agents in Release' },                       // Agents in Release
        { id: 'superGroupAgentsInSplit', icon: '👤', title: 'Agents in Split' },                            // Agents in Split
        { id: 'superGroupAnsweredCalls', icon: '📄', title: 'Answered Calls' },                             // Answered Calls
        { id: 'superGroupBriefReport', icon: '📄', title: 'Brief Report' },                                   // Brief Report
        { id: 'superGroupDailyAbandonedReport', icon: '📄', title: 'Daily Abandoned Report' },               // Daily Abandoned Report
        { id: 'superGroupDailyAcceptedReport', icon: '📄', title: 'Daily Accepted Report' },                 // Daily Accepted Report
        { id: 'superGroupDailyAnsweredReport', icon: '📄', title: 'Daily Answered Report' },                  // Daily Answered Report
        { id: 'superGroupDailyBriefReport', icon: '📄', title: 'Daily Brief Report' },                       // Daily Brief Report
        { id: 'superGroupQueuedCalls', icon: '📄', title: 'Queued Calls' },                                   // Queued Calls
        { id: 'superGroupLongestCallInQueue', icon: '📄', title: 'Longest Call In Queue' },                  // Longest Call In Queue
        { id: 'superGroupOutboundLongestPend', icon: '📄', title: 'Outbound Longest Pend' },                  // Outbound Longest Pend
      ]
    },
  ];



  // This map helps associate widget types with their categories for count updates
  widgetToCategoryMap: { [widgetType: string]: string } = {
    examplePieChart: 'agents', chart: 'agents',
    superGroupList: 'dnis', agentsList: 'dnis', groupList: 'dnis', dnisList: 'dnis',
    groupDetailFullInfo: 'ivr-applications', groupQueueInfo: 'ivr-applications', groupAgentsInfo: 'ivr-applications',
    ivrApplicationInfo: 'ivr-applications', ivrPortInfo: 'ivr-applications', groupAbandonedInfo: 'ivr-applications', briefAgents: 'agents'
  };

  onSearchChange(query: string): void {
    this.searchQuery = query;
  }

  onCategoryToggle(categoryId: string): void {
    const category = this.categories.find(c => c.id === categoryId);
    if (category) {
      category.isOpen = !category.isOpen;
    }
  }

  onWidgetSelectionChange(event: {widgetId: string, selected: boolean}): void {
    if (event.selected) {
      this.selectedWidgetTypes.add(event.widgetId);
    } else {
      this.selectedWidgetTypes.delete(event.widgetId);
    }
    this.updateCategoryCounts(event.widgetId, event.selected);
  }

  onWidgetThumbnailClick(_widgetId: string): void {
    // For now, clicking thumbnail does nothing if checkboxes are the main selection method.
  }

  onWidgetPreviewClick(widgetId: string): void {
    console.log('Preview clicked for widget:', widgetId);
  }

  updateCategoryCounts(widgetType: string, isSelected: boolean): void {
    const categoryKey = this.widgetToCategoryMap[widgetType];
    const category = this.categories.find(c => c.id === categoryKey);
    if (category) {
      if (isSelected) {
        category.selectedCount++;
      } else {
        category.selectedCount--;
      }
    }
  }


  resetSelections(): void {
    this.selectedWidgetTypes.clear();
    this.categories.forEach(category => {
      category.selectedCount = 0;
    });
  }

  addSelectedWidgets(): void {
    if (this.selectedWidgetTypes.size > 0) {
      this.widgetsSelected.emit(Array.from(this.selectedWidgetTypes));
    }
    // Optionally close menu after adding
    // this.closeMenu.emit();
  }

  // Helper to get the text for the "Add X Components" button
  get addComponentsButtonText(): string {
    const count = this.selectedWidgetTypes.size;
    return `Add ${count} Component${count === 1 ? '' : 's'}`;
  }

  getFilteredCategories(): CategoryData[] {
    if (!this.searchQuery.trim()) {
      return this.categories;
    }

    return this.categories.map(category => ({
      ...category,
      widgets: category.widgets.filter(widget =>
        widget.title.toLowerCase().includes(this.searchQuery.toLowerCase())
      )
    })).filter(category => category.widgets.length > 0);
  }
}


