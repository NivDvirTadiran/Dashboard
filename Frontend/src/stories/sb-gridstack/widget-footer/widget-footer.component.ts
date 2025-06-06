import { Component, Input, ChangeDetectionStrategy } from '@angular/core';
import { CommonModule, DatePipe } from '@angular/common';
import { FontAwesomeModule } from '@fortawesome/angular-fontawesome';
import { faClock } from '@fortawesome/free-solid-svg-icons';
import { FaIconLibrary } from '@fortawesome/angular-fontawesome';

@Component({
  selector: 'app-widget-footer',
  standalone: true,
  imports: [CommonModule, FontAwesomeModule, DatePipe],
  templateUrl: './widget-footer.component.html',
  styleUrls: ['./widget-footer.component.scss'],
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class WidgetFooterComponent {
  @Input() lastUpdated: number | Date | null = null;

  constructor(library: FaIconLibrary) {
    library.addIcons(faClock);
  }
}
