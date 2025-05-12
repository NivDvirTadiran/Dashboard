// also exported from '@storybook/angular' if you can deal with breaking changes in 6.1
import {componentWrapperDecorator, moduleMetadata, Meta, StoryFn,} from "@storybook/angular";
import CardComponent from "./card/card.component";
import { StoryInputComponent } from "./input/story-input.component";
import { ButtonContinueComponent } from "./input/button-continue/button-continue.component";
import { CommonModule } from "@angular/common";
import * as StoryInput from "./input/story-input.stories";

import {
  FormControl,
  FormGroup,
  FormsModule,
  ReactiveFormsModule,
  Validators,
} from "@angular/forms";


import LoginFormComponent from "./login-form.component";
import { TranslatePipe } from "../../pipes/translate/translate.pipe";

//import {TRANSLATION_PROVIDERS, TRANSLATIONS} from "../../../app/storybook/pipes/translate/translations";
import { TranslateService } from "../../pipes/translate/translate.service";

// More on default export: https://storybook.js.org/docs/angular/writing-stories/introduction#default-export
export default {
  title: "Design System/Login/LoginForm",
  component: LoginFormComponent,
  decorators: [
    moduleMetadata({
      declarations: [
        CardComponent,
        StoryInputComponent,
        ButtonContinueComponent,
        TranslateService,
        TranslatePipe,
      ],
      imports: [CommonModule, FormsModule, ReactiveFormsModule, ],
    }),
    componentWrapperDecorator(
      (story) => `<div style="margin: 3em">${story}</div>`,
    ),
  ],
  //.add('Testform', () => ({template: '<mods-undo-form></mods-undo-form>'})),
} as Meta;

const storyForm = new FormGroup({
  username: new FormControl("Telecom2", Validators.minLength(2)),
  password: new FormControl("T@diran2022", Validators.minLength(2)),
});

// More on component templates: https://storybook.js.org/docs/angular/writing-stories/introduction#using-args
const Template: StoryFn = (args) => ({
  props: {
    ...args,
    mForm: storyForm,
  },
});

export const Default = {
  render: Template,

  args: {
    storyInputs: [
      {
        ...StoryInput.Default.args?.["storyInput"],
        id: "1",
        title: "username",
        state: "USER NAME",
        icon: "./stories/assets/images/User2ldpi.svg",
        type: "text",
        placeholder: "Ex.Saul Ramirez",
        hide: false,
      },
      {
        ...StoryInput.Default.args?.["storyInput"],
        id: "2",
        title: "password",
        state: "PASSWORD",
        icon: "./stories/assets/images/LockIcon2ldpi.svg",
        type: "password",
        placeholder: "your_password",
        hide: false,
      },
    ],
  },
};

export const LoginForm = {
  render: Template,

  args: {
    // Shaping the stories through args composition.
    // Inherited data coming from the Default story.
    storyInputs: [
      {
        /*...AccountInput.Default.args?.['storyInput'],*/ id: "1",
        title: "username",
        state: "USER NAME",
        icon: "./assets/images/User2ldpi.png",
        type: "text",
        placeholder: "Ex.Saul Ramirez",
        hide: false,
      },
      {
        /*...AccountInput.Default.args?.['storyInput'],*/ id: "2",
        title: "password",
        state: "PASSWORD",
        icon: "./assets/images/LockIcon2ldpi.png",
        type: "password",
        placeholder: "your_password",
        hide: false,
      },
      // { id: '3', title: 'AccountInput 6 (pinned)', state: 'INPUT_PINNED' },
    ],
    mForm: [{ username: new FormControl("Telecom2") }],
  },
};

export const MainLoignForm = {
  render: Template,

  args: {
    // Shaping the stories through args composition.
    // Inherited data coming from the Default story.
    storyInputs: [
      ...Default.args["storyInputs"].slice(0, 2),
      // { id: '3', title: 'AccountInput 6 (pinned)', state: 'INPUT_PINNED' },
    ],
    validationForm: {
      username: new FormControl("Telecom2", Validators.minLength(2)),
      password: new FormControl("T@diran2022", Validators.minLength(2)),
    },
  },
};

export const LoignFailed = {
  render: Template,

  args: {
    // Shaping the stories through args composition.
    // Inherited data coming from the Default story.
    storyInputs: [
      ...Default.args["storyInputs"].slice(0, 2),
      // { id: '3', title: 'AccountInput 6 (pinned)', state: 'INPUT_PINNED' },
    ],
    validationForm: {
      username: new FormControl("Telecom2", Validators.minLength(2)),
      password: new FormControl("T@diran2023", Validators.minLength(2)),
    },
    isLoginFailed: true,
    loginErrorMessage: "Incorrect user name or password",
  },
};

export const Loading = {
  render: Template,

  args: {
    storyInputs: [
      ...Default.args["storyInputs"].slice(0, 2),
      // { id: '3', title: 'AccountInput 6 (pinned)', state: 'INPUT_PINNED' },
    ],
    isLoginFailed: false,
    isLoading: true,
  },
};
