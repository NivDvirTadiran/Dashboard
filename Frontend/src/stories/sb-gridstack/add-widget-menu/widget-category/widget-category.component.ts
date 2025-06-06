import { Component, EventEmitter, Input, Output, Type } from '@angular/core';
import { CommonModule } from '@angular/common';
import { WidgetThumbnailComponent, WidgetThumbnailData as OriginalWidgetThumbnailData } from '../widget-thumbnail/widget-thumbnail.component';
import {GSBaseWidget} from "../../base-widget/base-widget.component";

// Define WidgetThumbnailData with Type<any> for id
export interface WidgetThumbnailData extends Omit<OriginalWidgetThumbnailData, 'id'> {
  id: Type<any>; // Changed from string
}

export interface CategoryData {
  id: string; // Category ID remains string
  title: string;
  icon: string;
  isOpen: boolean;
  selectedCount: number;
  totalCount: number;
  widgets: WidgetThumbnailData[]; // Uses the updated WidgetThumbnailData
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
  @Input({}) selectedWidgets: Set<Type<any>> = new Set(); // Changed from Set<string>

  @Output() categoryToggle = new EventEmitter<string>();
  @Output() widgetSelectionChange = new EventEmitter<{widgetId: Type<any>, selected: boolean}>(); // Changed widgetId type
  @Output() widgetThumbnailClick = new EventEmitter<Type<any>>(); // Changed type
  @Output() widgetPreviewClick = new EventEmitter<Type<any>>(); // Changed type

  onHeaderClick(): void {
    this.categoryToggle.emit(this.category.id);
  }

  onWidgetSelectionChange(event: {widgetId: Type<any>, selected: boolean}): void {
    this.widgetSelectionChange.emit(event);
  }

  onWidgetThumbnailClick(widgetId: Type<any>): void {
    this.widgetThumbnailClick.emit(widgetId);
  }

  onWidgetPreviewClick(widgetId: Type<any>): void {
    this.widgetPreviewClick.emit(widgetId);
  }

  isWidgetSelected(widgetId: Type<any>): boolean {
    return this.selectedWidgets.has(widgetId);
  }

  getCountText(): string {
    return `${this.category.selectedCount}/${this.category.totalCount} selected`;
  }
}
