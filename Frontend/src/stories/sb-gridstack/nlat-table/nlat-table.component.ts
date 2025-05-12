import { Component, Input, OnChanges, SimpleChanges, ChangeDetectionStrategy, ChangeDetectorRef } from '@angular/core';
import { GroupListDataItemType } from '../../../app/services/emis-soap.service'; // Adjusted path
import {NgFor, NgIf } from '@angular/common';

@Component({
  selector: 'group-list-table',
  templateUrl: './nlat-table.component.html',
  styleUrls: ['./nlat-table.component.scss'],
  imports: [NgIf, NgFor],
  changeDetection: ChangeDetectionStrategy.OnPush
})
export class NlatTableComponent implements OnChanges {
  @Input() data: GroupListDataItemType[] | null = [];

  displayedColumns: string[] = ['grpId', 'grpName', 'grpNumber', 'grpEmailEnable'];

  constructor(private cdr: ChangeDetectorRef) { }

  ngOnChanges(changes: SimpleChanges): void {
    if (changes['data']) {
      console.log('GroupListTableComponent data received:', this.data);
      this.cdr.markForCheck(); // Trigger change detection when data changes
    }
  }
}
