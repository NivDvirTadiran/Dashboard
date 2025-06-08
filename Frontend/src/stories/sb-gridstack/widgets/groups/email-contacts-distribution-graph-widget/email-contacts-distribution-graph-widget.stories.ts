import { Meta, moduleMetadata, StoryFn } from "@storybook/angular";
import { CommonModule } from "@angular/common";
import { FormsModule } from "@angular/forms";
import { EmailContactsDistributionGraphWidgetComponent } from "./email-contacts-distribution-graph-widget.component";
import { StatusFilterComponent } from "./status-filter/status-filter.component";
import { FrameComponent } from "./frame-component/frame-component.component";
import { StatusSwitcherWithIcon } from "../status-switcher-with-icon/status-switcher-with-icon.component";

export default {
  title: "Design System/Dashboard/Widget/PieChart",
  component: EmailContactsDistributionGraphWidgetComponent,
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

const Template: StoryFn<EmailContactsDistributionGraphWidgetComponent> =  (args) => ({
  props: args,
});

export const WidgetPieChart_Clean = {
  render: Template,

  args: {
    mode: "Reply",
  },
};
