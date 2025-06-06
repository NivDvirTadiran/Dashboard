import { Component, Input, OnInit, AfterViewInit, ViewChild, ElementRef, OnDestroy, OnChanges, SimpleChanges } from '@angular/core';
import { CommonModule } from '@angular/common';
import { Chart, ChartConfiguration, ChartType, registerables } from 'chart.js';

// Register Chart.js components
Chart.register(...registerables);

export interface PieChartDataItem {
  label: string;
  value: number;
  color?: string; // Optional custom color
}

export interface PieChartConfig {
  showLegend?: boolean;
  showLegendValues?: boolean;
  showTooltips?: boolean;
  animation?: boolean;
  // Add other chart configuration options as needed
}

@Component({
  selector: 'app-gs-generic-pie',
  templateUrl: './gs-generic-pie.component.html',
  styleUrls: ['./gs-generic-pie.component.scss'],
  standalone: true,
  imports: [CommonModule]
})
export class GsGenericPieComponent implements OnInit, AfterViewInit, OnDestroy, OnChanges {
  @Input() data: PieChartDataItem[] = [];
  @Input() title: string = '';
  @Input() config: PieChartConfig = {
    showLegend: true,
    showLegendValues: false,
    showTooltips: true,
    animation: true
  };
  @Input() widgetId: string = ''; // To interact with specific widget manager if needed

  @ViewChild('chartCanvas', { static: false }) chartCanvas!: ElementRef<HTMLCanvasElement>;

  private chart: Chart | null = null;
  private defaultColors = [
    '#3D8ECF', // Primary blue
    '#1FC777', // Success green
    '#FF6B6B', // Error red
    '#FFD93D', // Warning yellow
    '#6C757D', // Secondary gray
    '#17A2B8', // Info cyan
    '#E83E8C', // Pink
    '#6610F2', // Purple
    '#FD7E14', // Orange
    '#20C997'  // Teal
  ];

  // Computed properties for template
  get showLegend(): boolean {
    return this.config.showLegend !== false;
  }

  get showLegendValues(): boolean {
    return this.config.showLegendValues === true;
  }

  constructor() {}

  ngOnInit(): void {
    // Initial setup if needed
  }

  ngAfterViewInit(): void {
    // Initialize chart after view is ready
    setTimeout(() => {
      this.initializeChart();
    }, 0);
  }

  ngOnChanges(changes: SimpleChanges): void {
    // Handle data changes
    if (changes['data'] && !changes['data'].firstChange) {
      this.updateChart();
    }
  }

  ngOnDestroy(): void {
    this.destroyChart();
  }

  private initializeChart(): void {
    if (!this.chartCanvas || !this.data || this.data.length === 0) {
      return;
    }

    const ctx = this.chartCanvas.nativeElement.getContext('2d');
    if (!ctx) {
      return;
    }

    const chartData = this.prepareChartData();
    
    const chartConfig: ChartConfiguration<'pie'> = {
      type: 'pie',
      data: chartData,
      options: {
        responsive: true,
        maintainAspectRatio: true,
        animation: this.config.animation !== false ? {} : false,
        plugins: {
          legend: {
            display: false // We're using custom legend
          },
          tooltip: {
            enabled: this.config.showTooltips !== false,
            callbacks: {
              label: (context) => {
                const label = context.label || '';
                const value = typeof context.parsed === 'number' ? context.parsed : 0;
                
                // Calculate total safely
                let total = 0;
                if (context.dataset && context.dataset.data) {
                  context.dataset.data.forEach((val) => {
                    if (typeof val === 'number') {
                      total += val;
                    }
                  });
                }
                
                const percentage = total > 0 ? ((value / total) * 100).toFixed(1) : '0';
                return `${label}: ${value} (${percentage}%)`;
              }
            }
          }
        }
      }
    };

    this.chart = new Chart(ctx, chartConfig);
  }

  private prepareChartData(): any {
    const labels = this.data.map(item => item.label);
    const values = this.data.map(item => item.value);
    const colors = this.data.map((item, index) => item.color || this.defaultColors[index % this.defaultColors.length]);

    return {
      labels: labels,
      datasets: [{
        data: values,
        backgroundColor: colors,
        borderColor: '#ffffff',
        borderWidth: 2,
        hoverBorderWidth: 3,
        hoverBorderColor: '#ffffff'
      }]
    };
  }

  private updateChart(): void {
    if (!this.chart) {
      this.initializeChart();
      return;
    }

    const chartData = this.prepareChartData();
    this.chart.data = chartData;
    this.chart.update();
  }

  private destroyChart(): void {
    if (this.chart) {
      this.chart.destroy();
      this.chart = null;
    }
  }

  // Helper methods for template
  getDefaultColor(item: PieChartDataItem): string {
    const index = this.data.indexOf(item);
    return this.defaultColors[index % this.defaultColors.length];
  }

  formatValue(value: number): string {
    // Format value with thousand separators
    return value.toLocaleString();
  }
}
