import {
  Component,
  Input,
  ViewChild,
  OnInit,
  AfterViewInit,
  ComponentFactoryResolver,
  ChangeDetectionStrategy,
  ApplicationRef,
  Output,
  EventEmitter,
  Type, ComponentRef
} from '@angular/core';
import { DynamicCompDirective } from '../dynamic-comp.directive';
import { PopoverOptions } from '../popover.interface'
import {ActionInput} from "../../action-input/action-input.interface";
import {NgClass} from "@angular/common";


@Component({
  selector: 'bubble-input',
  templateUrl: './bubble-input.component.html',
  standalone: true,
  imports: [
    NgClass,
    DynamicCompDirective
  ],
  styleUrls: ['./bubble-input.component.scss']
})
export class BubbleInputComponent implements OnInit, AfterViewInit{
  @Input() display: any;
  @Input() popover: any;
  @Input() data?: ActionInput[];
  @Input() header?: string;
  @Input()  options?: PopoverOptions;
  show: boolean = false;
  isDynamic: boolean = false;
  @ViewChild(DynamicCompDirective, {static: true}) content!: DynamicCompDirective;
  @Output() triggerDetectionChange: EventEmitter<void> = new EventEmitter<void> ();
  actionInputComponentRef?: ComponentRef<any>;
  constructor(private componentFactoryResolver: ComponentFactoryResolver) {

  }
  ngOnInit(): void {
    if (this.options && typeof this.options.content !== "string") {
      this.isDynamic = true;
      const componentFactory = this.componentFactoryResolver.resolveComponentFactory(<Type<any>>this.options.content);
      const viewContainerRef = this.content.viewContainerRef;
      viewContainerRef.clear();
      this.actionInputComponentRef = viewContainerRef.createComponent(componentFactory);
      this.actionInputComponentRef.instance.setConditions(this.data);
      this.actionInputComponentRef.instance.setHeader(this.header);
    }

  }
  ngAfterViewInit(): void {}

  showPopup() {
    if (this.data != undefined && this.data?.length > 0) {
      this.show = true;
      this.triggerDetectionChange.emit();
    }
  }

  hidePopup() {
    this.show = false;
    this.triggerDetectionChange.emit();
  }

  setData(data: ActionInput[]) {
    this.data = data;
    this.actionInputComponentRef?.instance.setConditions(this.data);
  }

  setHeader(header: string) {
    this.header = header;
    this.actionInputComponentRef?.instance.setHeader(this.header);
  }

  /*   loadCarComponent(){
     const _viewContainerRef = this.content.viewContainerRef;

     //removes all views in that container
     _viewContainerRef.clear();

     const componentFactory = this.componentFactoryResolver.resolveComponentFactory(<Type<any>>this.options?.content);

     //Create an instance of the compo
     /*    const carComponentRef = _viewContainerRef.createComponent<ActionInputComponent>(componentFactory);

         //Pass data to the component
         carComponentRef.instance.image = 'https://images.unsplash.com/photo-1494976388531-d1058494cdd8?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&auto=format&fit=crop&w=870&q=80';

   }*/
}
