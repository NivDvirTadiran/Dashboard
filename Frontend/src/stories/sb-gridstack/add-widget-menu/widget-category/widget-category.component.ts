import { Component, Input, Output, EventEmitter } from '@angular/core';
import { CommonModule } from '@angular/common';
import { WidgetThumbnailData, WidgetThumbnailComponent } from '../widget-thumbnail/widget-thumbnail.component';

export interface CategoryData {
  id: string;
  title: string;
  icon: string;
  isOpen: boolean;
  selectedCount: number;
  totalCount: number;
  widgets: WidgetThumbnailData[];
}

@Component({
  selector: 'app-widget-category',
  templateUrl: './widget-category.component.html',
  styleUrls: ['./widget-category.component.scss'],
  standalone: true,
  imports: [CommonModule, WidgetThumbnailComponent],
})
export class WidgetCategoryComponent {
  @Input() category!: CategoryData;
  @Input() selectedWidgets: Set<string> = new Set();

  @Output() categoryToggle = new EventEmitter<string>();
  @Output() widgetSelectionChange = new EventEmitter<{widgetId: string, selected: boolean}>();
  @Output() widgetThumbnailClick = new EventEmitter<string>();
  @Output() widgetPreviewClick = new EventEmitter<string>();

  onHeaderClick(): void {
    this.categoryToggle.emit(this.category.id);
  }

  onWidgetSelectionChange(event: {widgetId: string, selected: boolean}): void {
    this.widgetSelectionChange.emit(event);
  }

  onWidgetThumbnailClick(widgetId: string): void {
    this.widgetThumbnailClick.emit(widgetId);
  }

  onWidgetPreviewClick(widgetId: string): void {
    this.widgetPreviewClick.emit(widgetId);
  }

  isWidgetSelected(widgetId: string): boolean {
    return this.selectedWidgets.has(widgetId);
  }

  getCountText(): string {
    return `${this.category.selectedCount}/${this.category.totalCount} selected`;
  }
}
