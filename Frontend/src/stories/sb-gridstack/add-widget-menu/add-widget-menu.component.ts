import { Component, EventEmitter, Output } from '@angular/core';

@Component({
  selector: 'app-add-widget-menu',
  templateUrl: './add-widget-menu.component.html',
  styleUrls: ['./add-widget-menu.component.scss']
})
export class AddWidgetMenuComponent {
  @Output() widgetSelected = new EventEmitter<string>();
  @Output() closeMenu = new EventEmitter<void>();

  selectWidget(widgetType: string): void {
    this.widgetSelected.emit(widgetType);
  }
}
