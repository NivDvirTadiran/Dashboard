import { Component, input, Input, viewChild } from '@angular/core';


import {DashboardService, TableRow} from "../dashboard.service";
import { Subscription } from 'rxjs';




@Component({
  selector: 'gs-omni-dashboard',
  templateUrl: './gs-omni-dashboard.component.html',
  styleUrl: './gs-omni-dashboard.component.scss',
  standalone: false
})
export class GsOmniDashboardComponent {
  tableObj: TableRow[] = [];
  public subscription: Subscription;
/*
  constructor(public dashboardService: DashboardService) {
    this.tableObj = dashboardService.getTableData();
  }

  ngOnInit(): void {
    console.log("DashboardComponent Start");
    this.subscription = this.dashboardService.variable$?.subscribe(variable => {
      this.tableObj = variable;
    });
  }
*/
}
