import { Component, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { GSBaseWidget } from '../base-widget/base-widget.component';
import { GsDashboardWidgetManagerService } from '../gs-dashboard-widget-manager.service';

// Define an interface for the expected data structure for this widget
export interface OneViewData {
  mainText: string;
  value: string | number;
  // Potentially add other properties like unit, trend, etc. if needed in the future
}

@Component({
  selector: 'app-one-view-widget',
  templateUrl: './one-view-widget.component.html',
  styleUrls: ['./one-view-widget.component.scss'],
  standalone: true,
  imports: [CommonModule, GSBaseWidget]
})
export class OneViewWidgetComponent extends GSBaseWidget implements OnInit {

  // Strongly type the data if possible, or use 'any' if structure varies widely
  public contentData?: OneViewData;

  constructor(protected override widgetManager: GsDashboardWidgetManagerService) {
    super(widgetManager);
  }

  override ngOnInit(): void {
    super.ngOnInit(); // Call base class ngOnInit
    // Initialize contentData from widget.state?.data if available
    // The actual data fetching is handled by GSBaseWidget's fetchData mechanism
    if (this.widget && this.widget.state && this.widget.state.data) {
      this.contentData = this.widget.state.data as OneViewData;
    }
  }

  // Make fetchData public to match GSBaseWidget
  public override fetchData(): void {
    if (!this.widget || !this.widget.state) {
      if (this.widget && this.widget.state) {
          this.widget.state.loading = false;
      }
      return;
    }

    this.widget.state.loading = true;
    this.widget.state.error = undefined;
    console.log(`Fetching data for OneViewWidget: ${this.widget.id}`);

    // Simulate API call or use actual data fetching logic if provided by widgetManager or a service
    // For demonstration, we'll set some mock data.
    // In a real scenario, this would come from an actual data source via widgetManager or a dedicated service.
    setTimeout(() => {
      if (this.widget && this.widget.state) {
        // Mock data based on the Figma design (but in English)
        const mockData: OneViewData = {
          mainText: 'Opened Today',
          value: 7
        };
        // Assign fetched data to the widget's state
        if (this.widget && this.widget.state) {
          this.widget.state.data = mockData;
        }
        this.contentData = mockData;

        this.widget.state.loading = false;
        this.widget.state.lastUpdated = Date.now();
        console.log(`Data fetched for OneViewWidget: ${this.widget.id}`, this.widget.state.data);
      }
    }, 1000); // Simulate network delay
  }

  // Helper to get data, falling back to placeholders if not loaded
  get displayData(): OneViewData {
    if (this.widget?.state?.loading) {
      return { mainText: 'Loading...', value: '...' };
    }
    if (this.widget?.state?.error) {
      return { mainText: 'Error', value: '!' };
    }
    return this.contentData || { mainText: 'No data', value: '-' };
  }
}
