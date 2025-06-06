import { Component, Input, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';

export interface OneViewData {
  mainText?: string;
  value: number | string;
  unit?: string;
  subtitle?: string;
  trend?: number; // Percentage change
  icon?: string; // Icon class name
  iconColor?: string;
  valueColor?: string;
}

export interface OneViewConfig {
  showTrend?: boolean;
  decimals?: number;
  prefix?: string; // e.g., "$" for currency
  suffix?: string; // e.g., "%" for percentages
  thousandSeparator?: boolean;
  // Add other configuration options as needed
}

@Component({
  selector: 'app-gs-generic-one-view',
  templateUrl: './gs-generic-one-view.component.html',
  styleUrls: ['./gs-generic-one-view.component.scss'],
  standalone: true,
  imports: [CommonModule]
})
export class GsGenericOneViewComponent implements OnInit {
  @Input() data: OneViewData | null = null;
  @Input() config: OneViewConfig = {
    showTrend: true,
    decimals: 0,
    thousandSeparator: true
  };
  @Input() loading: boolean = false;
  @Input() error: string | null = null;
  @Input() widgetId: string = ''; // To interact with specific widget manager if needed

  // Computed properties for template
  get mainText(): string {
    return this.data?.mainText || '';
  }

  get unit(): string {
    return this.data?.unit || '';
  }

  get subtitle(): string {
    return this.data?.subtitle || '';
  }

  get icon(): string {
    return this.data?.icon || '';
  }

  get iconColor(): string {
    return this.data?.iconColor || '';
  }

  get valueColor(): string {
    return this.data?.valueColor || '';
  }

  get trend(): number | undefined {
    return this.data?.trend;
  }

  get showTrend(): boolean {
    return this.config.showTrend !== false;
  }

  get formattedValue(): string {
    if (!this.data || this.data.value === null || this.data.value === undefined) {
      return '-';
    }

    let value = this.data.value;
    
    // Handle string values
    if (typeof value === 'string') {
      // If it's a string that looks like a number, parse it
      const numValue = parseFloat(value);
      if (!isNaN(numValue)) {
        value = numValue;
      } else {
        // Return string as is if it's not a number
        return value;
      }
    }

    // Format number value
    if (typeof value === 'number') {
      let formatted = value.toFixed(this.config.decimals || 0);
      
      // Add thousand separators if enabled
      if (this.config.thousandSeparator !== false) {
        formatted = this.addThousandSeparators(formatted);
      }
      
      // Add prefix and suffix
      if (this.config.prefix) {
        formatted = this.config.prefix + formatted;
      }
      if (this.config.suffix) {
        formatted = formatted + this.config.suffix;
      }
      
      return formatted;
    }

    return String(value);
  }

  constructor() {}

  ngOnInit(): void {
    // Initial setup if needed
  }

  // Helper method to format trend value
  formatTrendValue(trend: number): string {
    const sign = trend > 0 ? '+' : '';
    return `${sign}${trend.toFixed(1)}`;
  }

  // Helper method to add thousand separators
  private addThousandSeparators(value: string): string {
    const parts = value.split('.');
    parts[0] = parts[0].replace(/\B(?=(\d{3})+(?!\d))/g, ',');
    return parts.join('.');
  }
}
