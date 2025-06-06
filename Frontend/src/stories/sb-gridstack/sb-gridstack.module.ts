import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { SbGridstackComponent } from './sb-gridstack.component';
import { IvrPortInfoWidgetComponent } from './ivr-port-info-widget/ivr-port-info-widget.component';
import { GsGenericGraphComponent } from './generic-widget-content/gs-generic-graph/gs-generic-graph.component';

@NgModule({
  declarations: [
    SbGridstackComponent,
    IvrPortInfoWidgetComponent,
    GsGenericGraphComponent
  ],
  imports: [
    CommonModule
  ],
  exports: [
    SbGridstackComponent
  ]
})
export class SbGridstackModule { }
