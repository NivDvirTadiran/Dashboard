import { moduleMetadata, StoryFn, Meta } from "@storybook/angular";
import Background1Component from "./background1.component";
import { CommonModule } from "@angular/common";
import {FormsModule} from "@angular/forms";
import {BrowserAnimationsModule} from "@angular/platform-browser/animations";

export default {
  title: "Design System/Login/Background1",
  component: Background1Component,
  parameters: {
    // More on Story layout: https://storybook.js.org/docs/angular/configure/story-layout
    layout: "fullscreen",
  },
  decorators: [
    moduleMetadata({
      declarations: [],
      imports: [CommonModule, FormsModule, BrowserAnimationsModule, BrowserAnimationsModule],
    }),
  ],
  // More on argTypes: https://storybook.js.org/docs/angular/api/argtypes
  argTypes: {
    backgroundColor: { control: "color" },
  },
} as Meta;

/*
export const Vee = Template.bind({});
// More on args: https://storybook.js.org/docs/angular/writing-stories/args
Vee.args = {
  primary: true,
  label: 'Button',
};*/

const Template: StoryFn<Background1Component> =  (args) => ({
  props: args,
});

export const Primary = {
  render: Template,

  args: {
    primary: true,
    label: "Button7",
    backgroundColor: "#ff0",
    background: "linear-gradient(228.37deg, #EFF8FF 22.25%, #B0DCFF 88.18%)",
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
    label: "Button",
    backgroundColor: "#ff0",
  },
};

export const LoggedOut = {
  render: Template,
};

export const LoggedIn = {
  render: Template,
};
