import { Component } from '@angular/core';
import {DashboardService, TableRow} from "../../login2/login-main/dashboard.service";
import {Subscription} from "rxjs";

@Component({
  selector: 'app-dashboard',
  templateUrl: './dashboard.component.html',
  styleUrl: './dashboard.component.scss',
  standalone: false
})
export class DashboardComponent {
  tableObj: TableRow[] = [];
  public subscription: Subscription;

  constructor(public dashboardService: DashboardService) {
    this.tableObj = dashboardService.getTableData();
  }

  ngOnInit(): void {
    console.log("DashboardComponent Start");
    this.subscription = this.dashboardService.variable$?.subscribe(variable => {
      this.tableObj = variable;
    });
  }
}
