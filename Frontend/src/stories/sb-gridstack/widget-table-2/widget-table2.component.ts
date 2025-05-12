import {Component, HostBinding, Input, Optional} from "@angular/core";
import {TableRow} from "src/app/dashboard/dashboard.service";
import {AsyncPipe, NgForOf} from "@angular/common";
import {map, of} from "rxjs";





@Component({
  selector: "widget-table-2",
  templateUrl: "./widget-table2.component.html",
  styleUrls: ["./widget-table2.component.scss"],
  imports: [
    NgForOf
  ],
  standalone: true
})
export class WidgetTable2Component {
    @HostBinding("style.display") display = "contents";

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
        return this.tableObj?.slice(1,this.tableObj.length);
    }

    fieldNames: { fields: ['Value1', 'Value2', 'Value3'] };



}
