import { Component, Input, OnDestroy, OnInit, inject } from '@angular/core';
import { CommonModule, NgIf } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { BaseWidget } from 'gridstack/dist/angular';
import { GsDashboardWidgetManagerService, WidgetConfig, WidgetState } from '../gs-dashboard-widget-manager.service';
import { Subscription, interval, finalize, tap } from 'rxjs';
import { FontAwesomeModule } from '@fortawesome/angular-fontawesome';
import { faSyncAlt, faEllipsisV, faFilter, faTimes } from '@fortawesome/free-solid-svg-icons';
import { EmisSoapService, AgentsListReturnType, AgentsListDataItemType, AgentListItemDto, DnisListItemDto } from 'src/app/services/emis-soap.service';

// Define a more specific config type for widgets that support agent selection
export interface WidgetConfigWithAgentSelection extends WidgetConfig {
  config?: {
    selectedAgentIds?: string[];
    selectedDnisIds?: string[];
    description?: string; // Added description to config
    [key: string]: any; // Allow other config properties
  };
}

export interface AgentSelectItem {
  id: string;
  name: string;
  selected: boolean;
}

@Component({
  selector: 'app-base-widget',
  templateUrl: './base-widget.component.html',
  styleUrls: ['./base-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, NgIf, FormsModule, FontAwesomeModule]
})
export class GSBaseWidget extends BaseWidget implements OnInit, OnDestroy {
  @Input() widget!: WidgetConfigWithAgentSelection; // Use the more specific type
  protected dataSubscription: Subscription | undefined;

  public showSettingsModal = false;
  public editableWidgetName = '';
  public editableWidgetDescription = '';

  // FontAwesome icons
  faSyncAlt = faSyncAlt;
  faFilter = faFilter;
  faEllipsisV = faEllipsisV;
  faTimes = faTimes;

  // Agent selection
  agentsForSelection: (AgentListItemDto & { selected: boolean })[] = [];
  isLoadingAgents = false;

  // DNIS selection
  dnisForSelection: (DnisListItemDto & { selected: boolean })[] = [];
  isLoadingDnis = false;

  // Inject EmisSoapService - make it protected
  protected emisSoapService = inject(EmisSoapService);

  constructor(protected widgetManager: GsDashboardWidgetManagerService) { super(); }

  ngOnInit(): void {
    if (this.widget) {
      this.startDataRefresh();
      this.editableWidgetName = this.widget.title || '';
      // Ensure config exists before trying to access properties on it
      if (!this.widget.config) {
        this.widget.config = {};
      }
      this.editableWidgetDescription = this.widget.config?.description || '';
    }
  }

  ngOnDestroy(): void {
    this.stopDataRefresh();
  }

  protected startDataRefresh(): void {
    this.stopDataRefresh(); // Ensure no previous subscription is active
    if (this.widget.updateInterval > 0) {
      this.dataSubscription = interval(this.widget.updateInterval).subscribe(() => {
        this.fetchData();
      });
    }
    this.fetchData(); // Initial data fetch
  }

  protected stopDataRefresh(): void {
    if (this.dataSubscription) {
      this.dataSubscription.unsubscribe();
      this.dataSubscription = undefined;
    }
  }

  // This method is intended to be overridden by child components
  // Child components should call super.fetchData() if they want this base behavior
  public fetchData(): void {
    if (!this.widget || !this.widget.state) return;

    this.widget.state.loading = true;
    this.widget.state.error = undefined;
    console.log(`Base fetchData for widget: ${this.widget.id}. Child should override for specific data.`);
    // Simulate base loading, child will provide actual data
    setTimeout(() => {
      if (this.widget.state) {
        this.widget.state.loading = false; // Child should set this to false after its data is fetched
        // this.widget.state.lastUpdated = Date.now(); // Child should set this
      }
    }, 500);
  }

  refreshWidget(widgetId: string): void {
    console.log(`Refreshing widget: ${widgetId}`);
    this.fetchData();
  }

  getWidgetState(widgetId: string): WidgetState | undefined {
    return this.widgetManager.getWidgetState(widgetId);
  }

  toggleSettingsModal(): void {
    this.showSettingsModal = !this.showSettingsModal;
    if (this.showSettingsModal) {
      this.editableWidgetName = this.widget?.title || '';
      this.editableWidgetDescription = this.widget.config?.description || '';

      if (this.widget.type === 'brief-agents-widget' && this.agentsForSelection.length === 0 && !this.isLoadingAgents) {
        this.loadAgentsForSelection();
      }
    }
  }

  protected loadAgentsForSelection(): void {
    if (!this.widget || this.widget.type !== 'brief-agents-widget') return;

    this.isLoadingAgents = true;
    this.emisSoapService.getAgentsList().pipe(
      tap(agentsList => console.log('AgentsList raw response (expected array):', agentsList)),
      finalize(() => this.isLoadingAgents = false)
    ).subscribe({
      next: (agentsList: AgentsListDataItemType[]) => { // Expecting an array directly
        if (agentsList && Array.isArray(agentsList)) {
          const selectedIds = new Set(this.widget.config?.selectedAgentIds || []);
          this.agentsForSelection = agentsList.map((agent: AgentsListDataItemType) => ({
            id: agent.agentId.toString(), // Use agentId from AgentsListDataItemType
            name: agent.agentName,         // Use agentName from AgentsListDataItemType
            selected: selectedIds.has(agent.agentId.toString())
          }));
        } else {
          this.agentsForSelection = [];
          console.warn('No agents returned or invalid format for brief-agents-widget selection. Expected an array.');
        }
      },
      error: (err) => {
        console.error('Error loading agents for selection:', err);
        this.agentsForSelection = [];
        // Optionally, display an error message in the modal
      }
    });
  }

  closeSettingsModal(): void {
    this.showSettingsModal = false;
  }

  saveSettings(): void {
    if (this.widget) {
      this.widget.title = this.editableWidgetName;
      if (!this.widget.config) {
        this.widget.config = {};
      }
      this.widget.config.description = this.editableWidgetDescription;

      if (this.widget.type === 'brief-agents-widget') {
        this.widget.config.selectedAgentIds = this.agentsForSelection
          .filter(agent => agent.selected)
          .map(agent => agent.id);
        console.log('Saved selectedAgentIds:', this.widget.config.selectedAgentIds);
      } else if (this.widget.type === 'brief-dnis-widget') {
        this.widget.config.selectedDnisIds = this.dnisForSelection
          .filter(dnis => dnis.selected)
          .map(dnis => dnis.id);
        console.log('Saved selectedDnisIds:', this.widget.config.selectedDnisIds);
      }

      this.widgetManager.setWidgetConfig(this.widget);
      console.log('Widget settings saved:', this.widget);
      this.fetchData(); // Trigger data refresh for the current widget
    }
    this.closeSettingsModal();
  }
}
