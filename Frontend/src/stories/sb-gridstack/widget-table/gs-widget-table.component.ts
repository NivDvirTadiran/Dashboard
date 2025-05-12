import {Component, HostBinding, Input} from "@angular/core";
import {TableRow} from "src/app/login2/login-main/dashboard.service";
import {ScrollingModule} from '@angular/cdk/scrolling';
import {MatTable} from "@angular/material/table";
import { NgForOf } from "@angular/common";
import {BaseWidget, GridstackComponent, NgGridStackOptions, NgGridStackWidget} from 'gridstack/dist/angular';

@Component({
  selector: "gs-widget-table",
  templateUrl: "./gs-widget-table.component.html",
  styleUrls: ["./gs-widget-table.component.scss"],
  standalone: true,
  imports: [ScrollingModule, MatTable, NgForOf]
})
export class GsWidgetTableComponent extends BaseWidget {
    @HostBinding("style.display") display = "table";

    /** Variant props */
    @Input() state: string = "Hover";

    @Input() tableObj!: TableRow[];


    public tableObjHeader(): string[] {
        //console.log("keys: "+Object.keys(this.tableObj[0]).join(", \n"));
        return Object.keys((this.tableObj[0]));
    }

    public tableObjValues(row: TableRow): (string | 0)[] {
        return Object.values(row);
    }

    get tableObjBody(): TableRow[] {
        return this.tableObj?.slice(0,this.tableObj.length);
    }



}
