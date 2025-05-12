import {combineLatest, map, of} from "rxjs";
import {Component, EventEmitter, HostBinding, Input, OnDestroy, OnInit, Output, Renderer2} from "@angular/core";
import {CdkDrag, CdkDragEnd, CdkDragMove, CdkDragStart} from "@angular/cdk/drag-drop";
import { CommonModule } from "@angular/common";




@Component({
  selector: "win-template",
  templateUrl: "./win-template.component.html",
  styleUrls: ["./win-template.component.scss"],
  standalone: true,
  imports: [CommonModule, CdkDrag]
})
export class WinTemplateComponent implements OnInit, OnDestroy {
    public minWidth = 1;
    public maxWidth = 50;
    public minHeight = 1;
    public maxHeight = 50;

    private initialWidth: number;
    private initialHeight: number;
    private initialPointerPosition = { x: 0, y: 0 };

    @Input() size: { width: number; height: number }; // Dimensions of the widget
    @Input() resizeDirection: 'x' | 'y' | 'both' = 'x';

    @Output() widthChange = new EventEmitter<number>();
    @Output() heightChange = new EventEmitter<number>();
    @Output() onWidgetResize = new EventEmitter<{ width: number, height: number }>();

    constructor(private renderer: Renderer2) {}

    @HostBinding('class.horizontal') get isHorizontal(): boolean {
        return this.resizeDirection === 'x';
    }

    @HostBinding('class.vertical') get isVertical(): boolean {
        return this.resizeDirection === 'y';
    }

    @HostBinding('class.diagonal') get isDiagonal(): boolean {
        return this.resizeDirection === 'both';
    }

    ngOnInit(): void {
        // Nothing needed here
    }

    ngOnDestroy(): void {
        // Ensure we remove any classes we might have added
        this.renderer.removeClass(document.body, 'resizing-x');
        this.renderer.removeClass(document.body, 'resizing-y');
        this.renderer.removeClass(document.body, 'resizing-both');
    }

    dragStart(event: CdkDragStart): void {
        // Store initial dimensions
        this.initialWidth = this.size.width;
        this.initialHeight = this.size.height;

        if ("clientX" in event.event) { this.initialPointerPosition.x = event.event.clientX; }
        if ("clientY" in event.event) { this.initialPointerPosition.y = event.event.clientY; }
        // Add proper cursor styling
        this.renderer.addClass(document.body, `resizing-${this.resizeDirection}`);
    }

    /**
     * Handles the drag move event to resize the component.
     * @param { CdkDragMove } event - The drag move event.
     */
    dragMove(event: CdkDragMove) {

        const pointerPositionX$ = event.source.moved.pipe(map((e) => e.pointerPosition.x));
        const pointerPositionY$ = event.source.moved.pipe(map((e) => e.pointerPosition.y));

        combineLatest([
            of(this.initialPointerPosition.x), //pointerPositionX$.pipe(first()), // initial horizontal pointer position
            pointerPositionX$, // current horizontal pointer position
            of(this.initialPointerPosition.y), //pointerPositionY$.pipe(first()), // initial vertical pointer position
            pointerPositionY$, // current vertical pointer position
        ])

            .pipe(
                map(([initialX, currentX, initialY, currentY]) => {
                    const distanceX = currentX - initialX;
                    const distanceY = currentY - initialY;
                    const newWidth = Math.min(
                        Math.max(this.initialWidth + distanceX, this.minWidth),
                        this.maxWidth
                    );
                    const newHeight = Math.min(
                        Math.max(this.initialHeight + distanceY, this.minHeight),
                        this.maxHeight
                    );
                    return [newWidth, newHeight];
                })
            ).subscribe(([nWidth, nHeight]) => {
            if (this.isHorizontal || this.isDiagonal) {
                this.onWidgetResize.emit({ width: nWidth, height: nHeight })
            }
            if (this.isVertical || this.isDiagonal) {
                this.onWidgetResize.emit({ width: nWidth, height: nHeight })
            }
        });
    }

    dragEnd(event: CdkDragEnd): void {
        // Reset the drag handle position to its original location
        event.source.reset();

        // Remove cursor styling
        this.renderer.removeClass(document.body, `resizing-${this.resizeDirection}`);
    }
}
