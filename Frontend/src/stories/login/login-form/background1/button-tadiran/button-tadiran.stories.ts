// also exported from '@storybook/angular' if you can deal with breaking changes in 6.1
import { ButtonTadiranComponent } from './button-tadiran.component';
import {componentWrapperDecorator, moduleMetadata} from "@storybook/angular";
import {Story, Meta} from "@storybook/angular/types-6-0";
import {CommonModule} from "@angular/common";

// More on default export: https://storybook.js.org/docs/angular/writing-stories/introduction#default-export
export default {
  title: 'Design System/Atoms/Buttons/TadiranIcon',
  component: ButtonTadiranComponent,
  decorators: [
    moduleMetadata({
      declarations: [],
      imports: [CommonModule],
    }),
    /*componentWrapperDecorator<ButtonContinueComponent>((story) => `<div class="container" style="height: 200px">${story}</div>`),*/
  ],
  // More on argTypes: https://storybook.js.org/docs/angular/api/argtypes
} as Meta;

// More on component templates: https://storybook.js.org/docs/angular/writing-stories/introduction#using-args
const Template: Story<ButtonTadiranComponent> = (args: ButtonTadiranComponent) => ({
  props: args,
});

const Template2: Story<ButtonTadiranComponent> = (args: ButtonTadiranComponent) => ({
  props: args,
  /*template: '<storybook-button-tadiran ></storybook-button-tadiran>',*/

});



export const Gallery = Template2.bind({});
Gallery.args = {
  gallery: true,
};

export const Primary3 = Template2.bind({});
Primary3.args = {
  gallery: false,
};

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
