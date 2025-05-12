import { Directive, ViewContainerRef } from '@angular/core';

@Directive({
  standalone: true,
  selector: '[appDynamicComp]'
})
export class DynamicCompDirective {

  constructor(public viewContainerRef: ViewContainerRef) {}

}
