import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import {LoginMainComponent} from "./login-main/login-main.component";






const routes: Routes = [
  { path: 'login-main', component: LoginMainComponent },
  { path: '', redirectTo: 'login-main', pathMatch: 'full' },/**/
  { path: '**', redirectTo: 'login-main', },
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class Login2RoutingModule { }
