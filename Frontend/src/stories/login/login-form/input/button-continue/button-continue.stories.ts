// also exported from '@storybook/angular' if you can deal with breaking changes in 6.1
import { ButtonContinueComponent } from "./button-continue.component";
import { componentWrapperDecorator, moduleMetadata } from "@storybook/angular";
import { StoryFn, Meta } from "@storybook/angular";
import { CommonModule } from "@angular/common";
import {BrowserModule} from "@angular/platform-browser";
import {TranslateService} from "../../../../pipes/translate/translate.service";
import {TranslatePipe} from "../../../../pipes/translate/translate.pipe";
import {NgModule} from "@angular/core";

// More on default export: https://storybook.js.org/docs/angular/writing-stories/introduction#default-export
export default {
  title: "Design System/Login/LoginForm/ContinueButton",
  component: ButtonContinueComponent,
  decorators: [
    moduleMetadata({
      declarations: [TranslateService, TranslatePipe],
      imports: [CommonModule],
    }),
    componentWrapperDecorator<ButtonContinueComponent>((story) => `<div class="container" style="height: 200px">${story}</div>`),
  ],
  // More on argTypes: https://storybook.js.org/docs/angular/api/argtypes
} as Meta;

// More on component templates: https://storybook.js.org/docs/angular/writing-stories/introduction#using-args
const Template: StoryFn<ButtonContinueComponent> = (
  args
) => ({
  props: args,
});

const Template2: StoryFn<ButtonContinueComponent> = (
  args
) => ({
  props: args,
  /*template: '<storybook-button-tadiran ></storybook-button-tadiran>',*/
});

export const Gallery = {
  render: Template2,

  args: {
    gallery: true,
  },
};

export const Primary3 = {
  render: Template2,

  args: {
    gallery: false,
  },
};

export const Primary = {
  render: Template,

  args: {
    primary: true,
    label: "Continue",
  },

  parameters: {
    backgrounds: {
      values: [
        { name: "red", value: "#f00" },
        { name: "green", value: "#0f0" },
        { name: "blue", value: "#00f" },
      ],
    },
  },
};

export const Secondary = {
  render: Template,

  args: {
    label: "button2",
  },
};

export const Large = {
  render: Template,

  args: {
    size: "large",
    label: "button3",
  },
};

export const Small = {
  render: Template,

  args: {
    size: "small",
    label: "button4",
  },
};
