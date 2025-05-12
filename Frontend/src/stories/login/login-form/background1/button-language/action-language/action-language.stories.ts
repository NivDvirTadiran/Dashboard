// also exported from '@storybook/angular' if you can deal with breaking changes in 6.1
import { ActionLanguageComponent } from './action-language.component';
import {componentWrapperDecorator, moduleMetadata} from "@storybook/angular";
import {Story, Meta} from "@storybook/angular";
import {CommonModule} from "@angular/common";
import {BubbleAvatarComponent} from "../../directive/bubble-avatar/bubble-avatar.component";
import {PopoverDirective} from "../../directive/bubble-avatar/popover.directive";
import {FormsModule, ReactiveFormsModule} from "@angular/forms";

// More on default export: https://storybook.js.org/docs/angular/writing-stories/introduction#default-export
export default {
  title: 'Design System/Atoms/Action/LanguageAction',
  component: ActionLanguageComponent,
  decorators: [
    moduleMetadata({
      declarations: [BubbleAvatarComponent, PopoverDirective],
      imports: [CommonModule, ReactiveFormsModule],
    }),
    /*componentWrapperDecorator<ButtonContinueComponent>((story) => `<div class="container" style="height: 200px">${story}</div>`),*/
  ],
  // More on argTypes: https://storybook.js.org/docs/angular/api/argtypes
} as Meta;

// More on component templates: https://storybook.js.org/docs/angular/writing-stories/introduction#using-args
const Template: Story<ActionLanguageComponent> = (args: ActionLanguageComponent) => ({
  props: args,
});

const Template2: Story<ActionLanguageComponent> = (args: ActionLanguageComponent) => ({
  props: args,
  /*template: '<storybook-button-tadiran ></storybook-button-tadiran>',*/

});



export const Default = Template2.bind({});
Default.args = {
  availableLanguages: {
    eng: true,
    heb: true,
    esp: true,
    chi: true,
    rus: true
  }
  //gallery: true,
};

export const Minimal = Template2.bind({});
Minimal.args = {
  availableLanguages: {
    eng: true,
    heb: true,
    esp: false,
    chi: false,
    rus: false
  }
  //gallery: true,
};
