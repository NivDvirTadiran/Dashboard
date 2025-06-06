import { Component, Input, Output, EventEmitter, ChangeDetectionStrategy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FontAwesomeModule, FaIconLibrary } from '@fortawesome/angular-fontawesome';
import { faSyncAlt, faEllipsisV } from '@fortawesome/free-solid-svg-icons';

@Component({
  selector: 'app-widget-header',
  standalone: true,
  imports: [CommonModule, FontAwesomeModule],
  templateUrl: './widget-header.component.html',
  styleUrls: ['./widget-header.component.scss'],
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class WidgetHeaderComponent {
  @Input() title: string | undefined;
  @Input() widgetId: string | undefined;
  @Input() disableRefresh: boolean | null = false;

  @Output() refreshClicked = new EventEmitter<string>();
  @Output() settingsClicked = new EventEmitter<void>();

  constructor(library: FaIconLibrary) {
    library.addIcons(faSyncAlt, faEllipsisV);
  }

  onRefreshClick(): void {
    if (this.widgetId) {
      this.refreshClicked.emit(this.widgetId);
    }
  }

  onSettingsClick(): void {
    this.settingsClicked.emit();
  }
}
