import { Component, Input } from '@angular/core';

@Component({
  selector: 'gs-generic-graph',
  templateUrl: './gs-generic-graph.component.html',
  styleUrls: ['./gs-generic-graph.component.scss']
})
export class GsGenericGraphComponent {
  @Input() title = 'פניות פתוחות לפי זמן';
  
  // Sample data points for the graph
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
    return this.dataPoints.map((point, i) => {
      const x = (i / (this.dataPoints.length - 1)) * 100;
      const y = 100 - (point.value / this.maxYValue * 100);
      return `${x},${y}`;
    }).join(' ');
  }
}
