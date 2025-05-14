import { Component, Input, OnChanges, SimpleChanges, ChangeDetectionStrategy, ChangeDetectorRef } from '@angular/core';
import {
  BriefInfoRequestType,
  GroupListDataItemType,
  SuperGroupListItemType
} from '../../../app/services/emis-soap.service'; // Adjusted path
import {NgFor, NgIf } from '@angular/common';

@Component({
  selector: 'nla-table',
  templateUrl: './nlat-table.component.html',
  styleUrls: ['./nlat-table.component.scss'],
  imports: [NgIf, NgFor],
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class NlatTableComponent implements OnChanges {
  @Input() tableObj?: SuperGroupListItemType[] | null = [];

  displayedColumns: string[] = ['Group ID', 'Name'];


  constructor(private cdr: ChangeDetectorRef) { }

  ngOnChanges(changes: SimpleChanges): void {
    if (changes['data']) {
      console.log('GroupListTableComponent data received:', this.tableObj);
      this.cdr.markForCheck(); // Trigger change detection when data changes
    }
  }

}
