import { Meta, moduleMetadata, StoryFn } from "@storybook/angular";
import { CommonModule } from "@angular/common";
import { FormsModule } from "@angular/forms";
import { WidgetTableComponent } from "./widget-table.component";

export default {
  title: "Design System/Dashboard/Widget/Table",
  component: WidgetTableComponent,
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

const Template: StoryFn<WidgetTableComponent> =  (args) => ({
  props: args,
});

export const WidgetTable_Clean = {
  render: Template,

  args: {
    mode: "Reply",
  },
};
