import {Component, EventEmitter, Input, OnInit, Output} from '@angular/core';
import {TranslateService} from "../../pipes/translate/translate.service";




@Component({
  selector: 'storybook-background2',
  templateUrl: './background2Component.html',
  styleUrls: ['./background2Component.scss']
})
export default class Background2Component {


  /**
   * Is this the principal call to action on the login-main?
   */
  @Input()
  primary = true;

  /**
   * What background color to use
   */
  @Input()
  backgroundColor?: string;


  /**
   * What background color to use
   */
  @Input()
  background?: string;

  /**
   * Button contents
   *
   * @required
   */
  @Input()
  label = 'Page2Component';

  /**
   * Button contents
   *
   * @required
   */
  @Input()
  ACC_VERSION = '7.00.000';

  public get classes(): string[] {
    const mode = this.primary ? 'storybook-background1--primary' : 'storybook-background1--secondary';

    return ['storybook-background1', mode];
  }

  constructor(public _translate: TranslateService,) {}

  @Output() changeLang: EventEmitter<Event> = new EventEmitter();



}

