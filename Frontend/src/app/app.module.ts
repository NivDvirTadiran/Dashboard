import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppComponent } from './app.component';
import { HttpClientModule } from '@angular/common/http';
import {CommonModule} from '@angular/common';
import {FormsModule} from "@angular/forms";
import {RouterModule, RouterOutlet} from "@angular/router";
import { Routes } from '@angular/router';
import { GridstackComponent } from 'gridstack/dist/angular';

export const routes: Routes = [

  { path: 'login2', loadChildren: () => import('./login2/login2.module').then(m => m.Login2Module), },
  { path: 'dashboard', loadChildren: () => import('./dashboard/dashboard.module').then(m => m.DashboardModule), },


  { path: '', redirectTo: 'login2', pathMatch: 'full' },
  //{ path: "", component: AppComponent, pathMatch: "full" },
  //{ path: "login", component: LoginComponent, pathMatch: "full" }


];

@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    RouterModule.forRoot(routes),
    BrowserModule,
    HttpClientModule,
    FormsModule,
    CommonModule,
    RouterOutlet,
    GridstackComponent

  ],
  providers: [
    AppComponent
  ],
  exports: [RouterModule],
  bootstrap: [AppComponent]
})
export class AppModule { }
