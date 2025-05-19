import { Component, ViewEncapsulation, HostBinding } from "@angular/core";
import {StatusSwitcherWithIcon} from "../../status-switcher-with-icon/status-switcher-with-icon.component";


@Component({
  selector: "status-filter",
  templateUrl: "./status-filter.component.html",
  styleUrls: ["./status-filter.component.scss"],
  standalone: true,
  imports: [StatusSwitcherWithIcon]
})
export class StatusFilterComponent {
    @HostBinding("style.display") display = "contents";

    constructor() {}
}
