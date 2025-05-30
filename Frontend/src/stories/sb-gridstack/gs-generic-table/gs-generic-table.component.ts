import { Component, Input, OnInit, AfterViewInit, Renderer2, ElementRef, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';

export interface TableColumn {
  key: string; // Key to access data in the row object
  header: string; // Display name for the column header
  isSortable?: boolean; // Optional: if the column can be sorted
  // Add other column-specific properties if needed (e.g., cellRenderer, width)
}

export interface TableRowData {
  [key: string]: any; // Allows any string as a key and any type as a value
}

@Component({
  selector: 'app-gs-generic-table',
  templateUrl: './gs-generic-table.component.html',
  styleUrls: ['./gs-generic-table.component.scss'],
  standalone: true,
  imports: [CommonModule]
})
export class GsGenericTableComponent implements OnInit, AfterViewInit, OnDestroy {
  @Input() columns: TableColumn[] = [];
  @Input() data: TableRowData[] = [];
  @Input() widgetId: string = ''; // To interact with specific widget manager if needed

  // Properties for column resizing
  private isResizing = false;
  private currentResizingColumnIndex: number = -1;
  private startX: number = 0;
  private startWidth: number = 0;
  private tableElement: HTMLTableElement | null = null;
  private thElements: NodeListOf<HTMLTableCellElement> | null = null;

  // Listener cleanup functions
  private mouseMoveListener: (() => void) | null = null;
  private mouseUpListener: (() => void) | null = null;

  constructor(
    private renderer: Renderer2,
    private elRef: ElementRef<HTMLElement>
  ) {}

  ngOnInit(): void {
    // Initialization logic if needed
  }

  ngAfterViewInit(): void {
    setTimeout(() => {
      this.tableElement = this.elRef.nativeElement.querySelector('table.table');
      if (this.tableElement) {
        this.thElements = this.tableElement.querySelectorAll('thead th');
      }
    }, 0);
  }

  ngOnDestroy(): void {
    this.stopResizing(); // Clean up listeners
  }

  startResize(event: MouseEvent, columnIndex: number): void {
    event.preventDefault();
    event.stopPropagation();

    this.isResizing = true;
    this.currentResizingColumnIndex = columnIndex;

    if (!this.thElements || !this.thElements[columnIndex]) {
      this.tableElement = this.elRef.nativeElement.querySelector('table.table');
      if (this.tableElement) {
        this.thElements = this.tableElement.querySelectorAll('thead th');
      }
      if (!this.thElements || !this.thElements[columnIndex]) {
        console.error('Resizable column header not found for index:', columnIndex);
        this.isResizing = false;
        return;
      }
    }

    const thElement = this.thElements[this.currentResizingColumnIndex];
    this.startX = event.pageX;
    this.startWidth = thElement.offsetWidth;

    this.mouseMoveListener = this.renderer.listen('document', 'mousemove', (e) => this.onResizing(e));
    this.mouseUpListener = this.renderer.listen('document', 'mouseup', () => this.stopResizing());
  }

  private onResizing(event: MouseEvent): void {
    if (!this.isResizing || this.currentResizingColumnIndex < 0 || !this.thElements) {
      return;
    }

    const thElement = this.thElements[this.currentResizingColumnIndex];
    if (!thElement) return;

    const diffX = event.pageX - this.startX;
    const newWidth = this.startWidth + diffX;
    const minWidth = 50;

    if (newWidth > minWidth) {
      this.renderer.setStyle(thElement, 'width', `${newWidth}px`);
    }
  }

  private stopResizing(): void {
    if (!this.isResizing) {
      return;
    }
    this.isResizing = false;
    this.currentResizingColumnIndex = -1;

    if (this.mouseMoveListener) {
      this.mouseMoveListener();
      this.mouseMoveListener = null;
    }
    if (this.mouseUpListener) {
      this.mouseUpListener();
      this.mouseUpListener = null;
    }
  }

  // Helper to get cell value based on column key
  getCellData(row: TableRowData, columnKey: string): any {
    return row[columnKey];
  }
}
