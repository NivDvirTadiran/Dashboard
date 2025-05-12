import { Component, ElementRef, HostBinding, ViewChild } from "@angular/core";
import { Chart, ChartOptions } from 'chart.js';
import ChartDataLabels from 'chartjs-plugin-datalabels';

Chart.register(ChartDataLabels);

@Component({
  selector: "frame-component",
  templateUrl: "./frame-component.component.html",
  styleUrls: ["./frame-component.component.scss"],
  standalone: true
})
export class FrameComponent {
  @HostBinding("style.display") display = "contents";

  constructor() {}

  /** Template reference to the canvas element */
  @ViewChild('canvasEl') canvasEl: ElementRef;

  /** Canvas 2d context */
  private context: CanvasRenderingContext2D;

  ngAfterViewInit() {
    this.context = (
      this.canvasEl.nativeElement as HTMLCanvasElement
    ).getContext('2d');

    this.draw();
  }

  /**
   * Draws something using the context we obtained earlier on
   */
  private draw() {
    const data = {
      labels: ['צ׳אט באתר (46%)', 'דוא״ל (20%)', 'Whatsapp (16%)', '(12%) sms', 'שיחה קולית (5%)','מסנג׳ר (1%)'],
      datasets: [{
        data: [46, 20, 16, 12, 5, 1],
        backgroundColor: [
          'rgba(255, 81, 81, 1)',
          'rgba(67, 165, 255, 1)',
          'rgba(250, 195, 33, 1)',
          'rgba(153, 99, 213, 1)',
          'rgba(250, 150, 33, 1)',
          'rgba(31, 199, 119, 1)'
        ],
        borderColor: [
          'rgba(255, 81, 81, 0.2)',
          'rgba(67, 165, 255, 0.2)',
          'rgba(250, 195, 33, 0.2)',
          'rgba(153, 99, 213, 0.2)',
          'rgba(250, 150, 33, 0.2)',
          'rgba(31, 199, 119, 0.2)'
        ],
        borderWidth: 0.1
      }]
    };

    const options: ChartOptions = {
      responsive: true,
      maintainAspectRatio: false,
      animations: {

        scale: {
          from: 0,
          to: 1,
          type: 'number',
        },
        rotate: {
          from: 0,
          to: 360,
          type: 'number',
        },

      },
      plugins: {
        legend: {
          position: 'bottom',
        },
        datalabels: {
          formatter: (value: number, context) => {
            const dataArr = context.chart.data.datasets[0].data as number[];
            const sum = dataArr.reduce((acc, curr) => acc + curr, 0);
            const percentage = ((value * 100) / sum).toFixed(2) + '%';
            return percentage;
          },
          color: 'white',
          font: {
            size: 14,
          },
        },
      },
    };

    new Chart(this.context, {
      type: 'doughnut',
      data: data,
      options: options
    });
  }
}
