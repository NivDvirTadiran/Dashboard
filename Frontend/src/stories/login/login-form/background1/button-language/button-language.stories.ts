// also exported from '@storybook/angular' if you can deal with breaking changes in 6.1
import { Story, Meta } from '@storybook/angular/types-6-0';
import { ButtonLanguageComponent } from './button-language.component';
import {moduleMetadata} from "@storybook/angular";
import {CommonModule} from "@angular/common";

// More on default export: https://storybook.js.org/docs/angular/writing-stories/introduction#default-export
export default {
  title: 'Design System/Atoms/Buttons/LanguageIcon',
  component: ButtonLanguageComponent,
  decorators: [
    moduleMetadata({
      declarations: [],
      imports: [CommonModule],
    }),
  ],
  // More on argTypes: https://storybook.js.org/docs/angular/api/argtypes
} as Meta;

// More on component templates: https://storybook.js.org/docs/angular/writing-stories/introduction#using-args
const Template: Story<ButtonLanguageComponent> = (args: ButtonLanguageComponent) => ({
  props: args,
});

export const Primary = Template.bind({});
// More on args: https://storybook.js.org/docs/angular/writing-stories/args
Primary.args = {
  primary: true,
  label: 'form button1',
};
Primary.parameters = {
  backgrounds: {
    values: [
      { name: 'red', value: '#f00' },
      { name: 'green', value: '#0f0' },
      { name: 'blue', value: '#00f' },
    ],
  },
};

export const Secondary = Template.bind({});
Secondary.args = {
  label: 'button2',
};


export const Large = Template.bind({});
Large.args = {
  size: 'large',
  label: 'button3',
};

export const Small = Template.bind({});
Small.args = {
  size: 'small',
  label: 'button4',
};
