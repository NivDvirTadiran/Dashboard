import { Component, Input, OnInit, AfterViewInit, Renderer2, ElementRef, OnDestroy } from '@angular/core';
import { CommonModule } from '@angular/common';
import {FaIconLibrary, FontAwesomeModule } from '@fortawesome/angular-fontawesome';
import { faFilter } from '@fortawesome/free-solid-svg-icons';

export interface TableColumn {
  key: string; // Key to access data in the row object
  header: string; // Display name for the column header
  isSortable?: boolean; // Optional: if the column can be sorted
  visible?: boolean; // Optional: if the column is currently visible
  isFilterable?: boolean; // Optional: if the column can be filtered (defaults to true)
  filterValue?: string; // Current filter value for this column
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
  imports: [CommonModule, FontAwesomeModule]
})
export class GsGenericTableComponent implements OnInit, AfterViewInit, OnDestroy {
  @Input() columns: TableColumn[] = [];
  @Input() data: TableRowData[] = [];
  @Input() widgetId: string = ''; // To interact with specific widget manager if needed

  // New inputs for enabling/disabling features
  @Input() enableSorting: boolean = true;
  @Input() enableFiltering: boolean = true;
  @Input() enableColumnToggle: boolean = true;

  // Properties for sorting
  public currentSortColumn: string | null = null;
  public sortDirection: 'asc' | 'desc' = 'asc';

  // Properties for filtering
  public columnFilters: Map<string, string> = new Map();
  public activeFilterColumnKey: string | null = null; // To show/hide individual column filter inputs
  public filteredData: TableRowData[] = [];

  // Properties for column visibility
  public displayColumns: TableColumn[] = []; // Columns currently displayed

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


  constructor(private renderer: Renderer2,
              private elRef: ElementRef<HTMLElement>,
              library: FaIconLibrary) {
    library.addIcons(faFilter);
  }

  ngOnInit(): void {
    this.initializeColumnFilters();
    this.updateDisplayColumns();
    this.applyFilters(); // Initial filter application
  }

  ngOnChanges(changes: any): void { // Using any for simplicity, can be SimpleChanges
    if (changes.columns) { // If column definitions change, re-initialize filters
      this.initializeColumnFilters();
    }
    if (changes.data || changes.columns) {
      this.updateDisplayColumns();
      this.applyFilters(); // Re-apply filters if data or columns change
    }
  }

  private initializeColumnFilters(): void {
    this.columnFilters.clear();
    this.columns.forEach(col => {
      // Ensure column is filterable (isFilterable !== false means true or undefined)
      if (col.isFilterable !== false) {
        this.columnFilters.set(col.key, col.filterValue || '');
      }
    });
  }

  private updateDisplayColumns(): void {
    this.displayColumns = this.columns.filter(col => col.visible !== false);
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

  // Sorting Logic
  sortTable(columnKey: string): void {
    if (!this.enableSorting) return;

    if (this.currentSortColumn === columnKey) {
      this.sortDirection = this.sortDirection === 'asc' ? 'desc' : 'asc';
    } else {
      this.currentSortColumn = columnKey;
      this.sortDirection = 'asc';
    }
    this.applySorting();
  }

  private applySorting(): void {
    if (!this.currentSortColumn) {
      this.applyFilters(); // If no sort, just ensure filters are applied
      return;
    }

    const sortedData = [...this.filteredData].sort((a, b) => {
      const valA = a[this.currentSortColumn!];
      const valB = b[this.currentSortColumn!];

      if (valA < valB) return this.sortDirection === 'asc' ? -1 : 1;
      if (valA > valB) return this.sortDirection === 'asc' ? 1 : -1;
      return 0;
    });
    this.filteredData = sortedData; // Update filteredData which is used by the template
  }

  // Filtering Logic
  toggleColumnFilterInput(columnKey: string, event?: MouseEvent): void {
    if (event) {
      event.stopPropagation(); // Prevent sortTable from triggering if filter icon is inside th
    }
    this.activeFilterColumnKey = this.activeFilterColumnKey === columnKey ? null : columnKey;
  }

  onColumnFilterChange(event: Event, columnKey: string): void {
    const inputElement = event.target as HTMLInputElement;
    const filterValue = inputElement.value.toLowerCase();
    this.columnFilters.set(columnKey, filterValue);

    // Optional: Update the column definition's filterValue if you want to persist this
    // This might be useful if columns are re-initialized or if parent components need this info
    const column = this.columns.find(col => col.key === columnKey);
    if (column) {
      column.filterValue = inputElement.value; // Store original case for display in input if needed
    }

    this.applyFilters();
  }

  private applyFilters(): void {
    if (!this.enableFiltering) {
      this.filteredData = [...this.data];
      if (this.currentSortColumn) {
        this.applySorting(); // Apply sorting if it's active
      }
      return;
    }

    let dataToFilter = [...this.data];
    let activeFiltersExist = false;

    this.columnFilters.forEach((filterVal, key) => {
      if (filterVal && filterVal.trim() !== '') {
        activeFiltersExist = true;
        const columnDef = this.columns.find(c => c.key === key);
        // Ensure the column is actually filterable according to its definition
        if (columnDef && columnDef.isFilterable !== false) {
          dataToFilter = dataToFilter.filter(row => {
            const cellValue = row[key];
            return cellValue != null && cellValue.toString().toLowerCase().includes(filterVal);
          });
        }
      }
    });

    this.filteredData = dataToFilter;

    // Re-apply sorting after filtering
    if (this.currentSortColumn) {
      this.applySorting();
    } else if (!activeFiltersExist && !this.currentSortColumn) {
      // If no active filters and no sort, ensure it's the original data order
      // This case is mostly covered by the initial [...this.data] if no filters are active.
      // However, if applySorting was called before and then filters cleared, this ensures reset.
      // this.filteredData = [...this.data]; // This line might be redundant depending on flow.
    }
  }

  // Column Toggle Logic
  toggleColumnVisibility(columnKey: string): void {
    if (!this.enableColumnToggle) return;

    const column = this.columns.find(col => col.key === columnKey);
    if (column) {
      column.visible = !(column.visible !== false); // Toggle, defaulting to true if undefined
      if (column.visible === false) {
        // If column is hidden, clear its filter
        if (this.columnFilters.has(columnKey)) {
          this.columnFilters.set(columnKey, ''); // Clear the filter value
          if (column.filterValue) {
            column.filterValue = ''; // Also clear it from the column definition if stored there
          }
        }
        // If the active filter input was for this column, hide it
        if (this.activeFilterColumnKey === columnKey) {
          this.activeFilterColumnKey = null;
        }
      }
      this.updateDisplayColumns();
      this.applyFilters(); // Data might need re-filtering/sorting
    }
  }

  isColumnVisible(columnKey: string): boolean {
    const column = this.columns.find(col => col.key === columnKey);
    return column ? column.visible !== false : true;
  }

  getVisibleColumns(): TableColumn[] {
    return this.columns.filter(col => col.visible !== false);
  }
}
