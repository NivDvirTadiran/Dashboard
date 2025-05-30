import type { Meta, StoryObj } from '@storybook/angular';
import { AddWidgetMenuComponent } from './add-widget-menu.component';

const meta: Meta<AddWidgetMenuComponent> = {
  title: 'Components/AddWidgetMenu',
  component: AddWidgetMenuComponent,
  parameters: {
    layout: 'fullscreen',
  },
  argTypes: {
    widgetsSelected: { action: 'widgets-selected' },
    closeMenu: { action: 'close-menu' },
  },
};

export default meta;
type Story = StoryObj<AddWidgetMenuComponent>;

export const Default: Story = {
  args: {},
};

export const WithInteraction: Story = {
  args: {},
  play: async ({ canvasElement }) => {
    // You can add interactions here to test the component behavior
  },
};