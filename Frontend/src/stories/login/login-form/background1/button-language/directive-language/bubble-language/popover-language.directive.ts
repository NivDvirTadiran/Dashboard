import {
  ComponentFactoryResolver,
  ComponentRef,
  Directive,
  ElementRef, EventEmitter,
  HostListener,
  Input, Output,
  ViewContainerRef
} from '@angular/core';
import { PopoverOptions } from '../popover.interface';
import {BubbleLanguageComponent} from "./bubble-language.component";
import {TranslateService} from "src/stories/pipes/translate/translate.service";

@Directive({
  selector: '[languagePopover]',
})
export class PopoverLanguageDirective {
  @Input("languagePopover") popover?: PopoverOptions;

  popoverComponentRef?: ComponentRef<BubbleLanguageComponent>;
  @HostListener('mouseover', ['$event']) onMouseOver($event: any){
    this.popoverComponentRef?.instance.setHeader(this.header);
    this.popoverComponentRef?.instance.setBubbleOn(this.bubbleOn);
    this.popoverComponentRef?.instance.showPopup();
    //this.eleRef.nativeElement.style.color = 'blue';
  }

  @HostListener('mouseleave', ['$event']) onLeave($event: any){
    this.popoverComponentRef?.instance.hidePopup();
  }

  @HostListener('click', ['$event']) onClick($event: any){
    this.popoverComponentRef?.instance.setHeader(this.header);
    this.popoverComponentRef?.instance.setBubbleOn(this.bubbleOn);
    this.popoverComponentRef?.instance.showPopup();
  }




  constructor(private eleRef: ElementRef,
              private el: ElementRef,
              private viewContainer: ViewContainerRef,
              private componentFactoryResolver: ComponentFactoryResolver) {}

  @Input() highlight: any;
  @Input() header: string = '';
  @Input() bubbleOn: boolean = false;
  @Input() _translate?: TranslateService;
  @Output() actionLanguage: EventEmitter<any> = new EventEmitter<any> ();
  @Output() languageButton: EventEmitter<any> = new EventEmitter<any> ();


/*
  @HostListener('mouseover') onMouseOver() {
    this.popoverComponentRef?.instance.showPopup();
    this.eleRef.nativeElement.style.color = this.colorName;
  }*/


  ngOnInit(): void {
    const factory =  this.componentFactoryResolver.resolveComponentFactory(BubbleLanguageComponent);

    const comp = factory.create(this.viewContainer.injector);
    comp.instance.display = "I test some content";
    comp.instance.popover = this.popover?.content;
    comp.instance.options = this.popover;
    comp.instance.header = this.header;
    comp.instance.bubbleOn = this.bubbleOn;
    comp.instance._translate = this._translate;

    this.popoverComponentRef = comp;
    this.el.nativeElement.classList.add("wrapper");
    this.el.nativeElement.appendChild(comp.location.nativeElement);

    comp.hostView.detectChanges();

    comp.instance.triggerDetectionChange.subscribe(() =>  {
      comp.hostView.detectChanges();
    });

    comp.instance.actionLanguage.subscribe(($event: any) => {
      this.actionLanguage.emit($event);
    });
  }

}
