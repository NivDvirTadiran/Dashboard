import {Component, EventEmitter, OnInit, Output} from '@angular/core';
import {TranslateService} from "src/stories/pipes/translate/translate.service";
import {AppConfig} from "src/app/app.config";
import { AvailableLanguages } from './available-languages.model'

@Component({
  selector: 'action-lang',
  templateUrl: './action-language.component.html',
  styleUrls: ['./action-language.component.css']
})
export class ActionLanguageComponent implements OnInit {



  bubbleOn?: boolean;
  actionHeader?: string = "";
  _translate!: TranslateService;

  @Output() actionLanguage = new EventEmitter<any>();

  availableLanguages: AvailableLanguages = {
                                            eng: true,
                                            heb: true,
                                            esp: false,
                                            chi: false,
                                            rus: false
                                          };


  changeLanguage(newLang: any){
    console.log("lang set to: "+newLang);
    this._translate.use(newLang);
    AppConfig.translateLanguage.lang = newLang;
    AppConfig.translateLanguage.dir = (newLang == 'heb' ? 'rtl' : 'ltr');
  }


  constructor(/*public _translate: TranslateService*/) {}

  ngOnInit() {
    this._translate?.use(AppConfig.translateLanguage.lang);
  }

  public setHeader(actionHeader: string) {
    this.actionHeader = actionHeader;
  }

  public setBubbleOn(bubbleOn: boolean) {
    this.bubbleOn = bubbleOn;
  }

  public setTranslate(_translate: TranslateService) {
    this._translate = _translate;
  }


}
