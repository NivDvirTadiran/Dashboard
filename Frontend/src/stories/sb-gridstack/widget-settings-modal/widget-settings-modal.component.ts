import { Component, OnInit, Input, Output, EventEmitter, OnChanges, SimpleChanges } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { FaIconComponent } from '@fortawesome/angular-fontawesome';
import { faTimes } from '@fortawesome/free-solid-svg-icons';

export interface AgentSelectItem {
  id: string;
  name: string;
  selected: boolean;
  // Add other relevant agent properties if needed by template
}

export interface DnisSelectItem {
  id: string;
  name: string;
  number: string;
  selected: boolean;
  // Add other relevant DNIS properties if needed by template
}

export type SelectItem = AgentSelectItem | DnisSelectItem;

// Define a type for the widget input for better type safety
export interface DashboardWidget {
  id: string;
  name: string;
  description?: string;
  type: string;
  settings?: any;
}

@Component({
  selector: 'app-widget-settings-modal',
  standalone: true,
  imports: [CommonModule, FormsModule, FaIconComponent],
  templateUrl: './widget-settings-modal.component.html',
  styleUrls: ['./widget-settings-modal.component.scss']
})
export class WidgetSettingsModalComponent implements OnInit, OnChanges {
  @Input() widget: DashboardWidget | null = null;
  @Input() entitiesForSelection: SelectItem[] = [];
  @Input() isLoading: boolean = false;

  @Output() settingsSaved = new EventEmitter<Partial<DashboardWidget> & { settings?: any }>();
  @Output() modalClosed = new EventEmitter<void>();

  editableWidgetName: string = '';
  editableWidgetDescription: string = '';


  faTimes = faTimes;

  constructor() {}

  ngOnInit(): void {
    if (this.widget) {
      this.populateFormFromWidget(this.widget);
    }
  }

  ngOnChanges(changes: SimpleChanges): void {
    if (changes['widget'] && changes['widget'].currentValue) {
      this.populateFormFromWidget(changes['widget'].currentValue);
    }
  }

  private populateFormFromWidget(widget: DashboardWidget): void {
    this.editableWidgetName = widget.name || '';
    this.editableWidgetDescription = widget.description || '';
  }

  saveSettings(): void {
    if (!this.widget) return;

    const updatedSettings: any = { ...(this.widget.settings || {}) };

    updatedSettings.selectedAgentIds = this.entitiesForSelection
      .filter(entity => entity.selected)
      .map(entity => entity.id);


    this.settingsSaved.emit({
      name: this.editableWidgetName,
      description: this.editableWidgetDescription,
      settings: updatedSettings
    });
  }

  closeModal(): void {
    this.modalClosed.emit();
  }
}
