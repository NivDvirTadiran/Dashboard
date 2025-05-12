import { Meta, moduleMetadata, StoryFn } from "@storybook/angular";
import { CommonModule } from "@angular/common";
import { FormsModule } from "@angular/forms";
import { StatusFilterComponent } from "./widget-pie-chart/status-filter/status-filter.component";
import { FrameComponent } from "./widget-pie-chart/frame-component/frame-component.component";
import { StatusSwitcherWithIcon } from "./status-switcher-with-icon/status-switcher-with-icon.component";
import { SbGridstackComponent } from "./sb-gridstack.component";

export default {
  title: "Design System/Dashboard",
  component: SbGridstackComponent,
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

const Template: StoryFn<SbGridstackComponent> =  (args) => ({
  props: args,
});

export const Dashboard_Clean = {
  render: Template,

  args: {
    mode: "Reply",
  },
};
