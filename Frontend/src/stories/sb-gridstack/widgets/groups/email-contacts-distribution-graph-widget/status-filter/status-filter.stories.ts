import { Meta, moduleMetadata, StoryFn } from "@storybook/angular";
import { CommonModule } from "@angular/common";
import { FormsModule } from "@angular/forms";
import { StatusFilterComponent } from "./status-filter.component";
import { StatusSwitcherWithIcon } from "../../status-switcher-with-icon/status-switcher-with-icon.component";
import { FrameComponent } from "../frame-component/frame-component.component";

export default {
  title: "Design System/Dashboard/Widget/PieChart/StatusFilter",
  component: StatusFilterComponent,
  parameters: {
    // More on Story layout: https://storybook.js.org/docs/angular/configure/story-layout
    layout: "centered",
  },
  decorators: [
    moduleMetadata({
      declarations: [
        StatusFilterComponent,
        FrameComponent,
        StatusSwitcherWithIcon,
      ],
      imports: [CommonModule, FormsModule],
    }),
  ],
} as Meta;

const Template: StoryFn<StatusFilterComponent> =  (args) => ({
  props: args,
});

export const StatusFilter_Clean = {
  render: Template,

  args: {
    mode: "Reply",
  },
};
