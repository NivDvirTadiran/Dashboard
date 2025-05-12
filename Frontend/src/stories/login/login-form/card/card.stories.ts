// also exported from '@storybook/angular' if you can deal with breaking changes in 6.1
import { StoryFn, Meta } from "@storybook/angular";
import Card from "./card.component";
import { StoryInputComponent } from "../input/story-input.component";
import { componentWrapperDecorator, moduleMetadata } from "@storybook/angular";
import { CommonModule } from "@angular/common";

// More on default export: https://storybook.js.org/docs/angular/writing-stories/introduction#default-export
export default {
  title: "Design System/Login/LoginForm/Card",
  component: Card,
  decorators: [
    moduleMetadata({
      declarations: [StoryInputComponent],
      imports: [CommonModule],
    }),
    componentWrapperDecorator(
      (story) => `<div id="main-login-card" class="col-md-12">${story}</div>`,
    ),
  ],
  // More on argTypes: https://storybook.js.org/docs/angular/api/argtypes
  argTypes: {
    backgroundColor: { control: "color" },
    size: { control: "select" },
  },
} as Meta;

// More on component templates: https://storybook.js.org/docs/angular/writing-stories/introduction#using-args
const Template: StoryFn<Card> = (args) => ({
  props: args,
});

export const Primary = {
  render: Template,

  args: {
    primary: true,
    label: "Button",
  },
};

export const Secondary = {
  render: Template,

  args: {
    size: "large",
    label: "Button",
  },
};
