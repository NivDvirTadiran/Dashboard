import { DragDropModule } from "@angular/cdk/drag-drop";
import {
    AfterViewInit,
    Component,
    ElementRef,
    EventEmitter,
    HostBinding,
    Input,
    OnChanges,
    OnDestroy,
    Output,
    SimpleChanges,
    ViewChild
} from "@angular/core";
import Chart from "chart.js/auto";

@Component({
  selector: "widget-pie-doughnut",
  templateUrl: "./widget-pie-doughnut.component.html",
  styleUrls: ["./widget-pie-doughnut.component.scss"],
  standalone: true,
  imports: [DragDropModule]
})
export class WidgetPieDoughnutComponent implements AfterViewInit, OnChanges, OnDestroy {
    @HostBinding("style.display") display = "contents";
    @HostBinding("style.width") get hostWidth() { return this.width ? `${this.width}px` : '100%'; }
    @HostBinding("style.height") get hostHeight() { return this.height ? `${this.height}px` : '100%'; }

    @Input() width: number;
    @Input() height: number;
    @Input() attId: string;
    @Output() widthChange = new EventEmitter<number>();
    @Output() heightChange = new EventEmitter<number>();

    @ViewChild('chartCanvas') chartCanvas: ElementRef;

    private chart: Chart;
    private resizeObserver: ResizeObserver;

    constructor(private elementRef: ElementRef) {}

    pieChartColors = [
        {
            backgroundColor: [
                'rgba(255, 81, 81, 0.2)',
                'rgba(67, 165, 255, 0.2)',
                'rgba(250, 195, 33, 0.2)',
                'rgba(153, 99, 213, 0.2)',
                'rgba(250, 150, 33, 0.2)',
                'rgba(31, 199, 119, 0.2)'
            ],
            hoverBackgroundColor: [
                'rgba(255, 81, 81, 1)',
                'rgba(67, 165, 255, 1)',
                'rgba(250, 195, 33, 1)',
                'rgba(153, 99, 213, 1)',
                'rgba(250, 150, 33, 1)',
                'rgba(31, 199, 119, 1)'
            ],
            borderWidth: 0.5,
        }
    ];

    // Pie
    public pieChartLabels:string[] = ['צ׳אט באתר (46%)', 'דוא״ל (20%)', 'Whatsapp (16%)', '(12%) sms', 'שיחה קולית (5%)','מסנג׳ר (1%)'];
    public pieChartData = [
        { data: [46, 20, 16, 12, 5, 1], label: 'קריאות שנפתחו לפי ערוצים' }
    ];
    public pieChartType:string = 'doughnut';

    pieChartOptions: any = {
        responsive: true,
        maintainAspectRatio: false,
        legend: {
            labels: {
                font: {
                    size: 10
                }
            },
            maxWidth: 150,
            position: 'bottom',
        },
    };

    ngAfterViewInit(): void {
        // Initialize the chart with current dimensions
        this.initChart();

        // Set up a ResizeObserver to handle container resizing
        this.setupResizeObserver();
    }

    ngOnChanges(changes: SimpleChanges): void {
        // If width or height changes, update chart size
        if ((changes.width && !changes.width.firstChange) ||
            (changes.height && !changes.height.firstChange)) {
            this.updateChartSize();
        }
    }

    ngOnDestroy(): void {
        // Cleanup chart and observer
        if (this.chart) {
            this.chart.destroy();
        }

        if (this.resizeObserver) {
            this.resizeObserver.disconnect();
        }
    }

    private initChart(): void {
        if (!this.chartCanvas) return;

        const ctx = this.chartCanvas.nativeElement.getContext('2d');

        // Apply initial size to canvas element
        if (this.width && this.height) {
            this.chartCanvas.nativeElement.width = this.width;
            this.chartCanvas.nativeElement.height = this.height;
        }

        this.chart = new Chart(ctx, {
            type: this.pieChartType as any,
            data: {
                labels: this.pieChartLabels,
                datasets: [
                    {
                        data: this.pieChartData[0].data,
                        label: this.pieChartData[0].label,
                        backgroundColor: this.pieChartColors[0].backgroundColor,
                        hoverBackgroundColor: this.pieChartColors[0].hoverBackgroundColor,
                        borderWidth: this.pieChartColors[0].borderWidth,
                    }
                ],
            },
            options: {
                ...this.pieChartOptions,
                responsive: true,
                maintainAspectRatio: false
            }
        });
    }

    private updateChartSize(): void {
        if (!this.chart) return;

        // Set new dimensions on the canvas element
        if (this.chartCanvas && this.width && this.height) {
            this.chartCanvas.nativeElement.style.width = `${this.width}px`;
            this.chartCanvas.nativeElement.style.height = `${this.height}px`;
        }

        // Update chart
        this.chart.resize();
    }

    private setupResizeObserver(): void {
        // Use ResizeObserver if available for better resize handling
        if (typeof ResizeObserver !== 'undefined') {
            this.resizeObserver = new ResizeObserver(entries => {
                if (!entries || !entries.length) return;

                // Need to run this outside Angular zone
                setTimeout(() => {
                    if (this.chart) {
                        this.chart.resize();
                    }
                }, 0);
            });

            this.resizeObserver.observe(this.elementRef.nativeElement);
        }
    }

    // events
    public chartClicked(e:any):void {
        console.log(e);
    }

    public chartHovered(e:any):void {
        console.debug(e);
    }
}
