import { Component, Input } from '@angular/core';
import { CommonModule } from '@angular/common'; // Import CommonModule
import { ChartDataResponse } from 'src/app/services/emis-soap.service'; // Assuming this is the correct path and interface

@Component({
  selector: 'app-gs-generic-graph', // Ensure selector matches usage in templates
  standalone: true, // Make it standalone
  imports: [CommonModule], // Import CommonModule for standalone components
  templateUrl: './gs-generic-graph.component.html',
  styleUrls: ['./gs-generic-graph.component.scss']
})
export class GsGenericGraphComponent {
  @Input() title = 'פניות פתוחות לפי זמן';
  @Input() chartData: ChartDataResponse | null = null; // Add chartData input

  // Sample data points for the graph (will be replaced or augmented by chartData)
  dataPoints = [
    { value: 50, time: '00:00' },
    { value: 38, time: '00:00' },
    { value: 0, time: '00:00' },
    { value: 119, time: '00:00' },
    { value: 179, time: '00:00' },
    { value: 121, time: '00:00' }
  ];
  
  // Y-axis values
  yAxisValues = [0, 25, 50, 75, 100];
  
  // Current time for the bottom-right display
  currentTime = new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
  
  // Max value for scaling
  get maxYValue() {
    return Math.max(...this.yAxisValues, ...this.dataPoints.map(p => p.value));
  }
  
  // Generate SVG polyline points
  getLinePoints(): string {
    // TODO: Adapt this to use this.chartData instead of this.dataPoints
    // For now, keeping the old logic to avoid breaking if chartData is not immediately available
    // or if a more complex mapping is needed.
    const pointsToUse = this.chartData?.datasets?.[0]?.data?.map((val, idx) => ({ value: val, time: this.chartData?.labels?.[idx] || '' })) || this.dataPoints;
    if (!pointsToUse || pointsToUse.length === 0) return '';

    return pointsToUse.map((point: any, i: number) => { // Added 'any' and 'number' for type safety
      const x = (i / (pointsToUse.length - 1)) * 100;
      const y = 100 - (point.value / this.maxYValue * 100);
      return `${x},${y}`;
    }).join(' ');
  }
}
