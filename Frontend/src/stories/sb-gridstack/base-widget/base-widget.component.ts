import { Component, Input, OnDestroy, OnInit, inject } from '@angular/core';
import { CommonModule, NgIf } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { BaseWidget } from 'gridstack/dist/angular';
import { GsDashboardWidgetManagerService, WidgetConfig, WidgetState } from '../gs-dashboard-widget-manager.service';
import { Subscription, interval, finalize, tap } from 'rxjs';
import { EmisSoapService, AgentsListDataItemType, DnisListItemType } from 'src/app/services/emis-soap.service';
import { WidgetSettingsModalComponent } from '../widget-settings-modal/widget-settings-modal.component';
import { DashboardWidget as ModalDashboardWidget, AgentSelectItem, DnisSelectItem } from '../widget-settings-modal/widget-settings-modal.component';

@Component({
  selector: 'app-base-widget',
  templateUrl: './base-widget.component.html',
  styleUrls: ['./base-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, NgIf, FormsModule, WidgetSettingsModalComponent]
})
export class GSBaseWidget extends BaseWidget implements OnInit, OnDestroy {
  @Input() widget!: WidgetConfig;
  protected dataSubscription: Subscription | undefined;

  public showSettingsModal = false;
  public currentModalWidgetData: ModalDashboardWidget | null = null;

  protected entitiesForSelection: any[] = [];

  protected isLoading = false;

  protected emisSoapService = inject(EmisSoapService);

  constructor(protected widgetManager: GsDashboardWidgetManagerService) { super(); }

  ngOnInit(): void {
    if (this.widget) {
      this.startDataRefresh();
      if (!this.widget.config) {
        this.widget.config = {};
      }
    }
  }

  ngOnDestroy(): void {
    this.stopDataRefresh();
  }

  protected startDataRefresh(): void {
    this.stopDataRefresh();
    if (this.widget.updateInterval > 0) {
      this.dataSubscription = interval(this.widget.updateInterval).subscribe(() => {
        this.fetchData();
      });
    }
    this.fetchData();
  }

  protected stopDataRefresh(): void {
    if (this.dataSubscription) {
      this.dataSubscription.unsubscribe();
      this.dataSubscription = undefined;
    }
  }

  public fetchData(): void {
    if (!this.widget || !this.widget.state) return;

    this.widget.state.loading = true;
    this.widget.state.error = undefined;
    console.log(`Base fetchData for widget: ${this.widget.id}. Child should override for specific data.`);
    setTimeout(() => {
      if (this.widget.state) {
        this.widget.state.loading = false;
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

  openSettingsModal(): void {
    console.log('[GSBaseWidget] openSettingsModal: ENTERED. Widget ID:', this.widget?.id);
    if (!this.widget) {
      console.warn('[GSBaseWidget] openSettingsModal: No widget defined, exiting.');
      return;
    }
    this.prepareModalData();

    this.currentModalWidgetData = {
      id: this.widget.id,
      name: this.widget.title || '',
      description: this.widget.config?.description || '',
      type: this.widget.type,
      settings: { ...(this.widget.config || {}) }
    };


    this.showSettingsModal = true;
  }

  /**
   * Protected method for derived widgets to override.
   * This method is called when the settings modal is about to open.
   * Implementations should load any specific data required for the modal settings,
   * such as lists for selection (e.g., agents, DNIS).
   */
  protected prepareModalData(): void {
    console.log(`GSBaseWidget.prepareModalData called for widget type: ${this.widget?.type}. Override in derived class if modal data is needed.`);
  }

  handleModalClosed(): void {
    this.showSettingsModal = false;
    this.currentModalWidgetData = null;
  }

  handleSettingsSaved(updatedModalData: Partial<ModalDashboardWidget> & { settings?: any }): void {
    if (!this.widget) return;

    this.widget.title = updatedModalData.name || this.widget.title;

    if (!this.widget.config) {
      this.widget.config = {};
    }

    if (updatedModalData.description !== undefined) {
      this.widget.config.description = updatedModalData.description;
    }

    if (updatedModalData.settings) {
      if (updatedModalData.settings.selectedAgentIds !== undefined) {
        this.widget.config.selectedEntitiesIds = updatedModalData.settings.selectedAgentIds;
      }
    }

    this.widgetManager.setWidgetConfig(this.widget);
    console.log('Widget settings saved via modal:', this.widget);
    this.fetchData();
    this.handleModalClosed();
  }
}
