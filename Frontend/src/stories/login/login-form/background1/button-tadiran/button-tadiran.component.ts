import { NgClass, NgStyle } from '@angular/common';
import { Component, Input, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'storybook-button-tadiran',
  templateUrl: './button-tadiran.component.html',
  styleUrls: ['./button-tadiran.component.scss'],
  imports: [NgClass, NgStyle]
})
export class ButtonTadiranComponent {

  /**
   * Is this gallery attribute are set?
   */
  @Input()
  gallery = false;

  /**
   * Is this the principal call to action on the login-main?
   */
  @Input()
  primary = false;

  /**
   * What background color to use
   */
  @Input()
  backgroundColor?: string;

  /**
   * How large should the button be?
   */
  @Input()
  size: 'small' | 'medium' | 'large' = 'medium';

  /**
   * Button contents
   *
   * @required
   */
  @Input()
  label = 'accGateButton2';

  /**
   * Optional click handler
   */
  @Output()
  onClick = new EventEmitter<Event>();

  public get classes(): string[] {
    const mode = this.primary ? 'storybook-button-tadiran--primary' : 'storybook-button-tadiran--secondary';
    const galleryMode = this.gallery ? 'storybook-button-tadiran--set-in' : 'storybook-button-tadiran--set-out';

    return ['storybook-button-tadiran', `storybook-button-tadiran--${this.size}`, mode, galleryMode];
  }


}
