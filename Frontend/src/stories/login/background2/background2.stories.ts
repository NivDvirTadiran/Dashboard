import { moduleMetadata, Meta, StoryFn } from "@storybook/angular";
import Background2Component from "./background2Component";
import { CommonModule } from "@angular/common";

export default {
  name: "Background2",
  title: "Design System/Atoms/Background2",
  component: Background2Component,
  parameters: {
    layout: "fullscreen",
    backgrounds: {
      values: [
        { name: "red", value: "#f00" },
        { name: "green", value: "#0f0" },
        { name: "blue", value: "#00f" },
      ],
    },
  },
  decorators: [
    moduleMetadata({
      declarations: [
        /* ButtonLanguageComponent, ButtonTadiranComponent*/
      ],
      imports: [CommonModule],
    }),
  ],
  argTypes: {
    backgroundColor: { control: "color" },
  },
  args: {
    primary: true,
    label: "Button7",
    backgroundColor: "#ff0",
    background: "linear-gradient(228.37deg, #EFF8FF 22.25%, #B0DCFF 88.18%)",
  },
} as Meta<Background2Component>;

const Template: (args: Background2Component) => {
  props: Background2Component;
} = (args: Background2Component) => ({
  props: args,
});

export const Primary = Template.bind({
  primary: true,
  label: "Button7",
  backgroundColor: "#ff0",
  background: "linear-gradient(228.37deg, #EFF8FF 22.25%, #B0DCFF 88.18%)",
});

export const Secondary = Template.bind({
  parameters: {},
  args: {
    label: "Button",
    backgroundColor: "#ff0",
  },
});

export const LoggedOut = {
  render: Template,
};

export const LoggedIn = {
  render: Template,
};
