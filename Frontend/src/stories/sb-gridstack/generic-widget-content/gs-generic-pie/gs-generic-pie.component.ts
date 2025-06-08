import { Component, Input, OnInit, AfterViewInit, ViewChild, ElementRef, OnDestroy, OnChanges, SimpleChanges } from '@angular/core';
import { CommonModule } from '@angular/common';
import { Chart, ChartConfiguration, ChartType, registerables, ChartData } from 'chart.js'; // Import ChartData
import { ChartDataResponse, ChartDataset } from 'src/app/services/emis-soap.service'; // Import ChartDataResponse

// Register Chart.js components
Chart.register(...registerables);

export interface PieChartDataItem { // No longer needed if using ChartDataResponse
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
  selector: 'gs-generic-pie',
  templateUrl: './gs-generic-pie.component.html',
  styleUrls: ['./gs-generic-pie.component.scss'],
  standalone: true,
  imports: [CommonModule]
})
export class GsGenericPieComponent implements OnInit, AfterViewInit, OnDestroy, OnChanges {
  @Input() chartData: ChartDataResponse;
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

  public legendItems: { label: string; color: string; value?: number }[] = [];

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
      this.updateLegendItems();
    }, 0);
  }

  ngOnChanges(changes: SimpleChanges): void {
    // Handle data changes
    if (changes['chartData'] && !changes['chartData'].firstChange) { // Changed from 'data' to 'chartData'
      this.updateChart();
      this.updateLegendItems();
    }
  }

  ngOnDestroy(): void {
    this.destroyChart();
  }

  private initializeChart(): void {
    if (!this.chartCanvas || !this.chartData || !this.chartData.datasets || this.chartData.datasets.length === 0 || !this.chartData.datasets[0].data || this.chartData.datasets[0].data.length === 0) {
      console.warn('[GsGenericPieComponent] initializeChart: Canvas not ready or no data.');
      return;
    }

    const ctx = this.chartCanvas.nativeElement.getContext('2d');
    if (!ctx) {
      console.error('[GsGenericPieComponent] initializeChart: Failed to get 2D context.');
      return;
    }

    const chartData = this.prepareChartData();
    if (!chartData) { // Check if prepareChartData returned null
        console.warn('[GsGenericPieComponent] initializeChart: Prepared chart data is null.');
        return;
    }

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

  private prepareChartData(): ChartData<'pie', number[], string> | null {
    if (!this.chartData || !this.chartData.labels || !this.chartData.datasets || this.chartData.datasets.length === 0) {
      console.warn('[GsGenericPieComponent] prepareChartData: Insufficient chart data.');
      return null;
    }

    const datasetFromInput = this.chartData.datasets[0]; // This is ChartDataset<'pie', number[]> from chart.js
    const labels = this.chartData.labels || [];
    const values = datasetFromInput.data || [];

    let backgroundColors: string[];
    if (Array.isArray(datasetFromInput.backgroundColor) && datasetFromInput.backgroundColor.length >= labels.length && datasetFromInput.backgroundColor.every(c => typeof c === 'string')) {
      backgroundColors = datasetFromInput.backgroundColor as string[];
    } else if (typeof datasetFromInput.backgroundColor === 'string' && labels.length > 0) {
      // If a single color string is provided, use it for all, or decide on a strategy.
      // For pie charts, usually an array is expected. Fallback to default if single string for multiple labels.
      if (labels.length === 1) {
        backgroundColors = [datasetFromInput.backgroundColor];
      } else {
        backgroundColors = labels.map((_, index) => this.defaultColors[index % this.defaultColors.length]);
      }
    } else {
      backgroundColors = labels.map((_, index) => this.defaultColors[index % this.defaultColors.length]);
    }

    return {
      labels: labels,
      datasets: [{
        data: values,
        backgroundColor: backgroundColors,
        borderColor: datasetFromInput.borderColor || '#ffffff',
        borderWidth: datasetFromInput.borderWidth !== undefined ? datasetFromInput.borderWidth : 2,
        hoverBorderWidth: 3, // You can customize this
        hoverBorderColor: '#ffffff' // You can customize this
      }]
    };
  }

  private updateChart(): void {
    if (!this.chart) {
      this.initializeChart();
      return;
    }

    const chartData = this.prepareChartData();
    if (chartData) {
      this.chart.data = chartData;
      this.chart.update();
    }
  }

  private destroyChart(): void {
    if (this.chart) {
      this.chart.destroy();
      this.chart = null;
    }
  }

  private updateLegendItems(): void {
    if (!this.chartData || !this.chartData.labels || !this.chartData.datasets || this.chartData.datasets.length === 0) {
      this.legendItems = [];
      return;
    }
    const labels = this.chartData.labels as string[]; // Assuming labels are strings
    const datasetFromInput = this.chartData.datasets[0];
    const values = datasetFromInput.data as number[]; // Assuming data are numbers

    let backgroundColors: string[];
    if (Array.isArray(datasetFromInput.backgroundColor) && datasetFromInput.backgroundColor.length >= labels.length && datasetFromInput.backgroundColor.every(c => typeof c === 'string')) {
      backgroundColors = datasetFromInput.backgroundColor as string[];
    } else if (typeof datasetFromInput.backgroundColor === 'string' && labels.length > 0) {
      if (labels.length === 1) {
        backgroundColors = [datasetFromInput.backgroundColor];
      } else {
        backgroundColors = labels.map((_, index) => this.defaultColors[index % this.defaultColors.length]);
      }
    } else {
      backgroundColors = labels.map((_, index) => this.defaultColors[index % this.defaultColors.length]);
    }

    this.legendItems = labels.map((label, index) => ({
      label: label,
      color: backgroundColors[index], // Use the resolved backgroundColors
      value: values[index]
    }));
  }

  formatValue(value: number): string {
    // Format value with thousand separators
    return value.toLocaleString();
  }
}
