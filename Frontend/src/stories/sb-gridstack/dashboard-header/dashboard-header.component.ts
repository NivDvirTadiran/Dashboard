import { Component, EventEmitter, HostBinding, Output } from '@angular/core';
import { CommonModule } from '@angular/common';
import { MatIcon } from '@angular/material/icon';
import { MatListItemIcon } from '@angular/material/list';
import { FontAwesomeModule } from '@fortawesome/angular-fontawesome';
import { faHeadset } from '@fortawesome/free-solid-svg-icons';

@Component({
  selector: 'app-dashboard-header',
  standalone: true,
  imports: [MatIcon, /*MatListItemIcon,*/ CommonModule, FontAwesomeModule],
  templateUrl: './dashboard-header.component.html',
  styleUrls: ['./dashboard-header.component.scss']
})
export class DashboardHeaderComponent {
  @HostBinding("style.gap") display = "1rem";
  faPuzzlePiece = faHeadset;

  @Output() addWidget = new EventEmitter<void>();
  @Output() save = new EventEmitter<void>();
  @Output() load = new EventEmitter<void>();
  @Output() refreshAll = new EventEmitter<void>();

  constructor() { }

  presentAddWidgetOptions(): void {
    this.addWidget.emit();
  }

  saveLayout(): void {
    this.save.emit();
  }

  loadLayout(): void {
    this.load.emit();
  }

  refreshAllWidgetsInternal(): void {
    this.refreshAll.emit();
  }
}
