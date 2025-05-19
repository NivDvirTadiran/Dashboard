import { NgModule } from '@angular/core';

import Background1Component from "../../stories/login/login-form/background1/background1.component";
import {MatButton} from "@angular/material/button";
import {NgForOf, NgIf} from "@angular/common";
import { RouterModule, Routes } from '@angular/router';
import {GridstackDashboardComponent} from './gridstack-dashboard/gridstack-dashboard.component';
import {GridstackComponent, GridstackModule } from 'gridstack/dist/angular';
import {AngularSimpleComponent} from "./gridstack-dashboard/simple";
import {AngularNgForTestComponent} from "./gridstack-dashboard/ngFor";
import {AngularNgForCmdTestComponent} from "./gridstack-dashboard/ngFor_cmd";
import {AComponent, BComponent, CComponent, NComponent} from "./gridstack-dashboard/dummy.component";
import {GsOmniDashboardComponent} from "./gs-omni-dashboard/gs-omni-dashboard.component";
import {SbGridstackComponent} from "../../stories/sb-gridstack/sb-gridstack.component";



const routes: Routes = [
  { path: 'gridstack-dashboard', component: GridstackDashboardComponent },
  { path: 'gs-omni-dashboard', component: GsOmniDashboardComponent },
  { path: '', redirectTo: 'dashboard', pathMatch: 'full' },/**/
  { path: '**', redirectTo: 'dashboard', },
];


@NgModule({
  declarations: [

    GridstackDashboardComponent,
    AComponent, BComponent, CComponent, NComponent,
    AngularSimpleComponent, AngularNgForTestComponent, AngularNgForCmdTestComponent,

    GsOmniDashboardComponent
  ],
  imports: [RouterModule.forChild(routes),
    //Background1Component,
    GridstackComponent,
    Background1Component,
    MatButton,
    NgForOf, NgIf, SbGridstackComponent,
    GridstackModule
  ],
  exports: [

  ],
  providers: [],
  bootstrap: [GridstackDashboardComponent]
})
export class DashboardModule {

}

