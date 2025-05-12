// also exported from '@storybook/angular' if you can deal with breaking changes in 6.1
import {
  StoryFn,
  Meta,
  moduleMetadata,
  componentWrapperDecorator,
} from "@storybook/angular";
import { StoryInputComponent } from "./story-input.component";
import { CommonModule } from "@angular/common";
import {
  FormControl,
  FormGroup,
  FormsModule,
  ReactiveFormsModule,
  Validators,
} from "@angular/forms";
import { TranslateService } from "../../../pipes/translate/translate.service";
import { TranslatePipe } from "../../../pipes/translate/translate.pipe";

// More on default export: https://storybook.js.org/docs/angular/writing-stories/introduction#default-export
export default {
  title: "Design System/Login/LoginForm/StoryInput",
  component: StoryInputComponent,
  decorators: [
    moduleMetadata({
      declarations: [
        StoryInputComponent,
        TranslatePipe,
        TranslateService,
      ],
      imports: [CommonModule, FormsModule, ReactiveFormsModule],
    }),
    componentWrapperDecorator(
      (story) => `<div style="margin: 3em">${story}</div>`,
    ),
  ],
  argTypes: {
    direction: { control: { type: "radio" } },
  },
  excludeStories: /.*Data$/,
} as Meta;


const Template: StoryFn = (args) => ({
  props: {
    ...args,
  },
});

export const Default = {
  render: Template,

  args: {
    storyInput: {
      id: "1",
      title: "Ex.Saul Ramirez",
      state: "USER NAME",
      icon: "./assets/images/User2ldpi.png",
      type: "text",
      placeholder: "placeholder",
      hide: false,
    },
    /*currentForm: {
      username: new FormControl('ea6', Validators.minLength(2)),
      password: new FormControl('', []),
    },*/
    hideInput: false,
    direction: "rtl",
  },
};

export const Username = {
  render: Template,

  args: {
    ...Default.args,
    storyInput: {
      id: "2",
      title: "username",
      state: "USER NAME",
      icon: "./stories/assets/images/User2ldpi.svg",
      type: "text",
      placeholder: "Ex.Saul Ramirez",
      hide: false,
    },
    currentForm: {
      ...Default.args["currentForm"],
      password: new FormControl("ea6", Validators.minLength(2)),
    },
    direction: "rtl",
  },
};

export const Password = {
  render: Template,

  args: {
    storyInput: {
      id: "3",
      title: "password",
      state: "PASSWORD",
      icon: "./assets/images/LockIcon2ldpi.png",
      type: "password",
      placeholder: "your_password",
      hide: false,
    },
  },
};

export const NewPassword = {
  render: Template,

  args: {
    ...Default.args,
    storyInput: {
      id: "6",
      title: "password",
      state: "NEW PASSWORD",
      icon: "./assets/images/LockIcon2ldpi.png",
      type: "password",
      placeholder: "your_password",
      hide: false,
    },
  },
};

export const Email = {
  render: Template,

  args: {
    storyInput: {
      id: "4",
      title: "email",
      state: "EMAIL",
      icon: "./assets/images/AtSign3ldpi.png",
      type: "email",
      placeholder: "Ex: abc@example.com",
      hide: false,
    },
  },
};

export const Phone = {
  render: Template,

  args: {
    storyInput: {
      id: "5",
      title: "phone",
      state: "PHONE NUMBER FOR AUTHENTICATION",
      icon: "./stories/assets/images/Phone3ldpi.svg",
      type: "tel",
      placeholder: "Ex: +972547762084",
      hide: false,
    },
    hideInput: true,
  },
};
