import { Component, Input, Output, EventEmitter } from '@angular/core';
import { CommonModule } from '@angular/common';

export interface WidgetThumbnailData {
  id: string;
  icon: string;
  title: string;
  showPreview?: boolean;
}

@Component({
  selector: 'app-widget-thumbnail',
  templateUrl: './widget-thumbnail.component.html',
  styleUrls: ['./widget-thumbnail.component.scss'],
  standalone: true,
  imports: [CommonModule]
})
export class WidgetThumbnailComponent {
  @Input() widget!: WidgetThumbnailData;
  @Input() isSelected: boolean = false;

  @Output() selectionChange = new EventEmitter<{widgetId: string, selected: boolean}>();
  @Output() thumbnailClick = new EventEmitter<string>();
  @Output() previewClick = new EventEmitter<string>();

  onCheckboxChange(event: Event): void {
    const checkbox = event.target as HTMLInputElement;
    this.selectionChange.emit({
      widgetId: this.widget.id,
      selected: checkbox.checked
    });
  }

  onThumbnailClick(event: MouseEvent): void {
    const target = event.target as HTMLElement;
    if (target.classList.contains('widget-checkbox') ||
        target.classList.contains('preview-link') ||
        target.closest('.preview-link')) {
      return;
    }
    this.thumbnailClick.emit(this.widget.id);
  }

  onPreviewClick(event: Event): void {
    event.stopPropagation();
    this.previewClick.emit(this.widget.id);
  }
}
