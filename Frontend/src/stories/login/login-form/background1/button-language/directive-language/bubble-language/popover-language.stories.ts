import {Story, Meta, componentWrapperDecorator, moduleMetadata} from '@storybook/angular';
import {CommonModule} from "@angular/common";
import {PopoverLanguageDirective} from "./popover-language.directive";
import {FormControl, FormGroup, FormsModule, ReactiveFormsModule, Validators} from "@angular/forms";
import {ActionAvatarComponent} from "src/stories/actions/action-avatar/action-avatar.component";
import { PopoverOptions } from '../popover.interface';
import {BubbleLanguageComponent} from "./bubble-language.component";



export default {
  title: 'Design System/Atoms/Directives/BubbleLanguageDirective',
  component: BubbleLanguageComponent, // (2) don't forget it
  decorators: [
    moduleMetadata({
      declarations: [ PopoverLanguageDirective, ActionAvatarComponent, BubbleLanguageComponent],
      imports: [CommonModule, FormsModule, ReactiveFormsModule],
    }),
    componentWrapperDecorator(story => `<div style="margin: 9em">${story}</div>`),
  ],
} as Meta;



const mPopover: PopoverOptions = {
  content: ActionAvatarComponent
};

const Template: Story<PopoverLanguageDirective> = (args) => ({
  props: args,
  moduleMetadata: { // (3) don't forget it
    declarations: [PopoverLanguageDirective, ActionAvatarComponent, BubbleLanguageComponent],
    imports: [CommonModule, FormsModule, ReactiveFormsModule]
  },
  template: `<button [languagePopover] = "mPopover" > BubbleAvatarDirective Example </button>`
});

export const Default = Template.bind({});
Default.args = {
  popover: mPopover,
};
