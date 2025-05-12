import {
  Component,
  Input,
  ViewChild,
  OnInit,
  AfterViewInit,
  ComponentFactoryResolver,
  Output,
  EventEmitter,
  Type, ComponentRef
} from '@angular/core';
import { DynamicCompDirective } from '../dynamic-comp.directive';
import { PopoverOptions } from '../popover.interface'
import {ActionLanguageComponent} from "../../action-language/action-language.component";
import {TranslateService} from "src/stories/pipes/translate/translate.service";
import {NgClass, NgIf } from '@angular/common';


@Component({
  selector: 'bubble-avatar',
    templateUrl: './bubble-language.component.html',
  styleUrls: ['./bubble-language.component.scss'],
  imports: [NgIf, NgClass, DynamicCompDirective]
})
export class BubbleLanguageComponent implements OnInit, AfterViewInit{
  @Input() display: any;
  @Input() popover: any;
  @Input() bubbleOn?: boolean;
  @Input() header?: string;
  @Input()  options?: PopoverOptions;
  @Input() _translate?: TranslateService;
  show: boolean = false;
  isDynamic: boolean = false;
  @ViewChild(DynamicCompDirective, {static: true}) content!: DynamicCompDirective;
  @Output() triggerDetectionChange: EventEmitter<void> = new EventEmitter<void> ();
  @Output() actionLanguage: EventEmitter<any> = new EventEmitter<any> ();
  actionLanguageComponentRef?: ComponentRef<any>;
  constructor(private componentFactoryResolver: ComponentFactoryResolver) {

  }
  ngOnInit(): void {
    if (this.options && typeof this.options.content !== "string") {
      this.isDynamic = true;
      const componentFactory = this.componentFactoryResolver.resolveComponentFactory(<Type<any>>this.options.content);
      const viewContainerRef = this.content.viewContainerRef;
      viewContainerRef.clear();
      this.actionLanguageComponentRef = viewContainerRef.createComponent(componentFactory);
      this.actionLanguageComponentRef.instance.setHeader(this.header);
      this.actionLanguageComponentRef.instance.setBubbleOn(this.bubbleOn);
      this.actionLanguageComponentRef.instance.setTranslate(this._translate);
      this.actionLanguageComponentRef.instance.actionLanguage.subscribe(($event: any) => {
        this.actionLanguage.emit($event);
      });

    }
  }
  ngAfterViewInit(): void {}

  showPopup() {
    if (this.bubbleOn) {
      this.show = true;
      this.triggerDetectionChange.emit();
    }
  }

  hidePopup() {
    this.show = false;
    this.triggerDetectionChange.emit();
  }

  setHeader(header: string) {
    this.header = header;
    this.actionLanguageComponentRef?.instance.setHeader(this.header);
  }

  setBubbleOn(bubbleOn: boolean) {
    this.bubbleOn = bubbleOn;
  }

  loadCarComponent(){
    const _viewContainerRef = this.content.viewContainerRef;

    //removes all views in that container
    _viewContainerRef.clear();

    const componentFactory = this.componentFactoryResolver.resolveComponentFactory(<Type<any>>this.options?.content);

    //Create an instance of the component
    const carComponentRef = _viewContainerRef.createComponent<ActionLanguageComponent>(componentFactory);
/*
    //Pass data to the component
    carComponentRef.instance.image = 'https://images.unsplash.com/photo-1494976388531-d1058494cdd8?ixlib=rb-1.2.1&ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&auto=format&fit=crop&w=870&q=80';
*/
  }
}
