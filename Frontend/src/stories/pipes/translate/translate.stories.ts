import { Meta, moduleMetadata, StoryObj } from "@storybook/angular";
import { CommonModule } from "@angular/common";
import { TranslateService } from "./translate.service";
//import {action} from "@storybook/addon-actions";

const meta: Meta<TranslateService> = {
  title: "Design System/Atoms/Translate",
  component: TranslateService,
  parameters: {
    // More on Story layout: https://storybook.js.org/docs/angular/configure/story-layout
    layout: "fullscreen",
  },
  decorators: [
    moduleMetadata({
      declarations: [TranslateService],
      imports: [CommonModule],
    }),
  ],
};

export const actionsData = {
  //onPinInput: action('setLang'),
  //onArchiveInput: action('getLang'),
};

export default meta;
type Template = StoryObj;

export const Primary: Template = {
  parameters: {
    backgrounds: {
      colors: [{ name: "white", value: "#fff" }],
    },
  },
};
