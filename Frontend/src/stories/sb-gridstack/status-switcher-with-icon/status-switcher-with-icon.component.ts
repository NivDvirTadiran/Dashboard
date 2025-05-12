import {
    Component,
    ViewEncapsulation,
    HostBinding,
    Input,
} from "@angular/core";
import { NgStyle } from "@angular/common";


@Component({
  selector: "status-switcher-with-icon",
  templateUrl: "./status-switcher-with-icon.component.html",
  styleUrls: ["./status-switcher-with-icon.component.scss"],
  standalone: true,
  imports: [NgStyle]
})
export class StatusSwitcherWithIcon {
    @HostBinding("style.display") display = "contents";

    constructor() {}

    /** Value props */
    @Input() prop: string = "";
    @Input() iconStatus: string = "";
    /** Variant props */
    @Input() state: string = "Default";
    @Input() type: string = "Default";
    /** Style props */
    @Input() statusSwitcherWithIconWidth: string | number = "";
    @Input() divFontSize: string | number = "";
    @Input() divFontFamily: string | number = "";

    get statusSwitcherWithIconStyle() {
        return {
            width: this.statusSwitcherWithIconWidth,
        };
    }

    get div1Style() {
        return {
            "font-size": this.divFontSize,
            "font-family": this.divFontFamily,
        };
    }
}
