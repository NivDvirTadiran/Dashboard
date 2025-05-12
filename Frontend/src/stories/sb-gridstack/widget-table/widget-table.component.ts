import {Component, HostBinding, Input} from "@angular/core";
import {TableRow} from "src/app/dashboard/dashboard.service";
import {ScrollingModule} from '@angular/cdk/scrolling';
import { NgForOf } from "@angular/common";
import {BaseWidget} from 'gridstack/dist/angular';

@Component({
  selector: "widget-table",
  templateUrl: "./widget-table.component.html",
  styleUrls: ["./widget-table.component.scss"],
  standalone: true,
  imports: [ScrollingModule, NgForOf]
})
export class WidgetTableComponent {
    @HostBinding("style.display") display = "table";

    /** Variant props */
    @Input() state: string = "Hover";

    @Input() tableObj!: TableRow[];


    public tableObjHeader(): string[] {
        // Check if tableObj is valid and has elements
        if (this.tableObj && this.tableObj.length > 0 && this.tableObj[0]) {
            return Object.keys(this.tableObj[0]);
        }
        return []; // Return empty array if tableObj is invalid or empty
    }

    public tableObjValues(row: TableRow): (string | 0)[] {
        return Object.values(row);
    }

    get tableObjBody(): TableRow[] {
        // Check if tableObj is an array before slicing
        if (Array.isArray(this.tableObj)) {
            // Slice from the second element onwards (index 1)
            return this.tableObj.slice(1); 
        }
        return []; // Return an empty array if tableObj is not an array
    }
}
