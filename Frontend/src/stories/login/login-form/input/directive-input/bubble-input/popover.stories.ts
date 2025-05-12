import {Meta, componentWrapperDecorator, moduleMetadata, StoryObj} from '@storybook/angular';
import {CommonModule} from "@angular/common";
import {PopoverInputDirective} from "./popover-input.directive";
import {FormControl, FormGroup, FormsModule, ReactiveFormsModule, Validators} from "@angular/forms";
import { PopoverOptions } from 'src/stories/directive/popover.interface';
import {ActionInputComponent} from "src/stories/actions/action-input/action-input.component";
import {BubbleInputComponent} from "./bubble-input.component";



export default {
  title: 'Design System/Atoms/Directives/PopoverInputDirective',
  component: PopoverInputDirective, // (2) don't forget it
  decorators: [
    moduleMetadata({
      declarations: [ PopoverInputDirective, ActionInputComponent, BubbleInputComponent],
      imports: [CommonModule, FormsModule, ReactiveFormsModule],
    }),
    componentWrapperDecorator(story => `<div style="margin: 9em">${story}</div>`),
  ],
  args: {},
} as Meta<PopoverInputDirective>;

const mPopover: PopoverOptions = {
  content: ActionInputComponent
};

const Template: StoryObj<PopoverInputDirective> = ({
  args: {}
});

export const Default = Template;
Default.args = {
  popover: mPopover,
};
