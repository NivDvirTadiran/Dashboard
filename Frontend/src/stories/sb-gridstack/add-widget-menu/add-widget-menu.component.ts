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
        { id: 'briefAgents', icon: '💼', title: 'Brief Agents' },
        { id: 'examplePieChart', icon: '🥧', title: 'Example Pie Chart' },
        { id: 'chart', icon: '📈', title: 'Chart Widget (API)' }
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
        { id: 'superGroupList', icon: '👥', title: 'Super Group List' },
        { id: 'agentsList', icon: '🧑‍💼', title: 'Agents List' },
        { id: 'groupList', icon: '🏢', title: 'Group List' },
        { id: 'dnisList', icon: '📞', title: 'DNIS List' }
      ]
    },
    {
      id: 'groups',                                                                                           // Abandoned Calls                            // Daily Answered Report   one-view                // Queued Calls   one-view            // Waiting Calls In Queue  one-view
      title: 'Groups',                                                                                        // Above T.ASA   one-view                     // Daily Answered Calls Percentage  one-view       // Interflow Out  one-view            // Sample  one-view
      icon: 'ℹ️',                                                                                             // Accepted Calls   one-view                  // Daily Brief Report                              // Longest Call In Queue one-view
      isOpen: false,                                                                                          // Agents Distribution Graph                  // Daily Detailed Report                           // OACD STI Graph
      selectedCount: 0,                                                                                       // Agents MultiDistribution Graph             // Detailed Report                                 // Outbound Longest Pend   one-view
      totalCount: 6,                                                                                          // Agents Logged In   one-view                // Email Brief Report   table                      // Outbound Pending Calls  one-view
      widgets: [                                                                                              // Agents Report   table                      // Email Contacts Distribution Graph               // Overflow-Interflow Report
                                                                                                              // Answered Calls   one-view                  // Email Daily Brief Report   table                // Overview
        { id: 'groupDetailFullInfo', icon: '📄', title: 'Group Detail Full Info' },                           // Answered Calls Percentage  one-view        // Email Daily Detailed Report     ?               // Queue Status Graph
        { id: 'groupQueueInfo', icon: '🕒', title: 'Group Queue Info' },                                      // Average Q Time   one-view                  // Email Detailed Report      ?                    // Queued Calls Graph
        { id: 'groupAgentsInfo', icon: '👨‍👩‍👧‍👦', title: 'Group Agents Info' },                                    // Calls Distribution Graph                   // Email Overflow-Interflow Report    !?           // Queued OACD Calls Graph
        { id: 'ivrApplicationInfo', icon: '📱', title: 'IVR Application Info' },                              // Calls Overview Graph                       // Email Queued Contacts Graph                     // STI Graph
        { id: 'ivrPortInfo', icon: '🔌', title: 'IVR Port Info', showPreview: false },                        // Daily Abandoned Report  one-view           // Email STI Graph                                 // Status Brief Report
        { id: 'groupAbandonedInfo', icon: '🚫', title: 'Group Abandoned Info' }                               // Daily Accepted Report   one-view           // Hourly Report                                   // TSF Percentage
      ]
    },
    {
      id: 'ivr-applications',                                                                                 // ACD Distribution Graph
      title: 'IVR Applications',                                                                              // Active ACD Graph
      icon: 'ℹ️',                                                                                             // Active OACD Graph
      isOpen: false,                                                                                          // Brief Report
      selectedCount: 0,                                                                                       // Detailed Report
      totalCount: 7,                                                                                          // OACD Distribution Graph
      widgets: [
        { id: 'groupDetailFullInfo', icon: '📄', title: 'Group Detail Full Info' },
        { id: 'groupQueueInfo', icon: '🕒', title: 'Group Queue Info' },
        { id: 'groupAgentsInfo', icon: '👨‍👩‍👧‍👦', title: 'Group Agents Info' },
        { id: 'ivrApplicationInfo', icon: '📱', title: 'IVR Application Info' },
        { id: 'ivrPortInfo', icon: '🔌', title: 'IVR Port Info', showPreview: false },
        { id: 'groupAbandonedInfo', icon: '🚫', title: 'Group Abandoned Info' },
        { id: 'briefAgents', icon: '💼', title: 'Brief Agents' }
      ]
    },
    {
      id: 'super-groups',                                                                                     // Abandoned Calls               // Brief Report
      title: 'Super Groups',                                                                                  // Above T.ASA                   // Daily Abandoned Report
      icon: '📋',                                                                                             // Accepted Calls                // Daily Accepted Report
      isOpen: false,                                                                                          // Agents in ACD                 // Daily Answered Report
      selectedCount: 0,                                                                                       // Agents in IDLE                // Daily Brief Report
      totalCount: 4,                                                                                          // Agents_Loggedin               // Queued Calls
      widgets: [                                                                                              // Agents in Non ACD             // Longest Call In Queue
        { id: 'superGroupList', icon: '👥', title: 'Super Group List' },                                      // Agents in Release             // Outbound Longest Pend
        { id: 'agentsList', icon: '🧑‍💼', title: 'Agents List' },                                               // Agents in Split               // Outbound Pending Calls
        { id: 'groupList', icon: '🏢', title: 'Group List' },                                                 // Answered Calls
        { id: 'dnisList', icon: '📞', title: 'DNIS List' }                                                    // Average Q Time
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
