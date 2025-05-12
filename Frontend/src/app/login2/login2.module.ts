import { NgModule } from '@angular/core';

import { Login2RoutingModule } from './login2-routing.module';
import {LoginMainComponent} from './login-main/login-main.component';

import {FormsModule} from "@angular/forms";
import LoginFormComponent from "../../stories/login/login-form/login-form.component";
import Background1Component from "../../stories/login/login-form/background1/background1.component";
import {MatButton} from "@angular/material/button";
import {NgForOf} from "@angular/common";






@NgModule({
  declarations: [
    LoginMainComponent,
  ],
  imports: [
    Login2RoutingModule,
    LoginFormComponent,
    FormsModule,
    Background1Component,
    MatButton,
    NgForOf,
  ],
  exports: [

  ],
  providers: [],
  bootstrap: [LoginMainComponent]
})
export class Login2Module { }
