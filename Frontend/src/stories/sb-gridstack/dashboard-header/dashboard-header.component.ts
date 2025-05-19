import { Component, EventEmitter, Output } from '@angular/core';
import { CommonModule } from '@angular/common';

@Component({
  selector: 'app-dashboard-header',
  standalone: true,
  imports: [CommonModule],
  templateUrl: './dashboard-header.component.html',
  styleUrls: ['./dashboard-header.component.scss']
})
export class DashboardHeaderComponent {
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
