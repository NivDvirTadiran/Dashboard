import {Component, EventEmitter, Input, OnInit, Output} from '@angular/core';
import {NgClass, NgStyle} from "@angular/common";
import {ButtonLanguageComponent} from "./button-language/button-language.component";
import {ButtonTadiranComponent} from "./button-tadiran/button-tadiran.component";





@Component({
  selector: 'storybook-background1',
  templateUrl: './background1.component.html',
  standalone: true,
  imports: [
    NgClass, NgStyle, ButtonLanguageComponent, ButtonTadiranComponent
  ],
  styleUrls: ['./background1.component.scss']
})
export default class Background1Component {


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
  ACC_VERSION = '8.00.117';

  public get classes(): string[] {
    const mode = this.primary ? 'storybook-background1--primary' : 'storybook-background1--secondary';

    return ['storybook-background1', mode];
  }

  constructor() {}

  @Output() changeLang = new EventEmitter<Event>();

  @Input() _translate?: any;


}

