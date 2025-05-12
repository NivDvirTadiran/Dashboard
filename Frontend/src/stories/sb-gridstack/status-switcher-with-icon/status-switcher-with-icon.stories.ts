import { Meta, moduleMetadata, StoryFn } from "@storybook/angular";
import { CommonModule } from "@angular/common";
import { FormsModule } from "@angular/forms";
import { StatusSwitcherWithIcon } from "./status-switcher-with-icon.component";

export default {
  title: "Design System/Dashboard/StatusSwitcherWithIcon",
  component: StatusSwitcherWithIcon,
  parameters: {
    // More on Story layout: https://storybook.js.org/docs/angular/configure/story-layout
    layout: "centered",
  },
  decorators: [
    moduleMetadata({
      declarations: [],
      imports: [CommonModule, FormsModule],
    }),
  ],
} as Meta;

const Template: StoryFn<StatusSwitcherWithIcon> =  (args) => ({
  props: args,
});

export const FrameComponent_Clean = {
  render: Template,

  args: {
    mode: "Reply",
  },
};
