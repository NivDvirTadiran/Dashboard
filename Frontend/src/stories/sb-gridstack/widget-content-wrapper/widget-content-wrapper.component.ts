import { Component, Input, ChangeDetectionStrategy } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FontAwesomeModule } from '@fortawesome/angular-fontawesome'; // For error icon
import { faExclamationTriangle } from '@fortawesome/free-solid-svg-icons';
import { FaIconLibrary } from '@fortawesome/angular-fontawesome';

@Component({
  selector: 'app-widget-content-wrapper',
  standalone: true,
  imports: [CommonModule, FontAwesomeModule],
  templateUrl: './widget-content-wrapper.component.html',
  styleUrls: ['./widget-content-wrapper.component.scss'],
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class WidgetContentWrapperComponent {
  @Input() loading: boolean | null = false;
  @Input() error: string | null = null;

  constructor(library: FaIconLibrary) {
    library.addIcons(faExclamationTriangle);
  }
}
