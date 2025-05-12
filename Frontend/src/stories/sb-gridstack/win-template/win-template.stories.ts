import { Meta, moduleMetadata, StoryFn } from "@storybook/angular";
import { CommonModule } from "@angular/common";
import { FormsModule } from "@angular/forms";
import { WinTemplateComponent } from "./win-template.component";

export default {
  title: "Design System/Dashboard/WinTemplate",
  component: WinTemplateComponent,
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

const Template: StoryFn<WinTemplateComponent> =  (args) => ({
  props: args,
});

export const WinTemplate_Clean = {
  render: Template,

  args: {
    mode: "Reply",
  },
};
