import {
    Component,
    ViewEncapsulation,
    HostBinding,
    Input,
} from "@angular/core";
import {CdkDragMove} from "@angular/cdk/drag-drop";
import {SbGridstackComponent} from "../sb-gridstack.component";
import {WinTemplateComponent} from "../win-template/win-template.component";
import {FrameComponent} from "./frame-component/frame-component.component";
import {StatusFilterComponent} from "./status-filter/status-filter.component";
import { NgIf } from "@angular/common";

// parse ful url to ip,port, params
var parsedUrl = new URL(window.location.href);
var url = parsedUrl.hostname;
var port = parsedUrl.port;
var protocol = parsedUrl.protocol;
const AUTH_API = protocol + "//" + url + ":" + port+'/accRealTime/nlat/IVRApplications.OACD_Distribution_Graph.nlat?ivrapplicationId=1';

@Component({
  selector: "widget-pie-chart",
  templateUrl: "./widget-pie-chart.component.html",
  styleUrls: ["./widget-pie-chart.component.scss"],
  standalone: true,
  imports: [FrameComponent, StatusFilterComponent, NgIf]
})
export class WidgetPieChartComponent {
    @HostBinding("style.display") display = "contents";

    constructor(public parent: SbGridstackComponent) {}

    /** Value props */
    @Input() showWidgetPieChart: boolean = true;
    @Input() prop: string = "קריאות שנפתחו לפי ערוצים";
    @Input() showDiv: boolean = true;
    /** Variant props */
    @Input() property1: number = 1;
    @Input() state: string = "Default";
}
