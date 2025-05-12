import { Component, OnInit } from '@angular/core';
import {ActionInput} from "./action-input.interface";
import {TranslateService} from "src/stories/pipes/translate/translate.service";
import {TranslatePipe} from "../../../../pipes/translate/translate.pipe";
import {NgForOf, NgIf} from "@angular/common";



interface USERS {
  conditionName: String;
  presentingMessage: String;
  isFulfilled: boolean;
}

@Component({
  selector: 'action-input',
  templateUrl: './action-input.component.html',
  standalone: true,
  imports: [
    TranslatePipe,
    NgForOf,
    NgIf
  ],
  styleUrls: ['./action-input.component.css']
})
export class ActionInputComponent implements OnInit {

  actionInputs?: ActionInput[];
  actionHeader: string = 'The password must contain:';
  public direction: 'ltr' | 'rtl';

  constructor(public _translate: TranslateService) {
    this.direction = this._translate.currentDir;
  }

  ngOnInit() {
  }

  public async setConditions(actionInputs: ActionInput[]) {
    this.actionInputs = actionInputs;
  }

  public setHeader(actionHeader: string) {
    this.actionHeader = actionHeader;
  }
}
