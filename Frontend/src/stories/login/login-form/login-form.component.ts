import {Component, ElementRef, EventEmitter, Input, OnInit, Output, Renderer2, ViewChild} from '@angular/core';
import {AbstractControl, FormControl, FormGroup, ReactiveFormsModule, Validators} from '@angular/forms';
import { StoryInput } from "./input/story-input.model";
import {TranslatePipe} from "../../pipes/translate/translate.pipe";
import {StoryInputComponent} from "./input/story-input.component";
import {NgIf} from "@angular/common";
import {ButtonContinueComponent} from "./input/button-continue/button-continue.component";
import CardComponent from "./card/card.component";
import {BehaviorSubject} from "rxjs";


export declare type Direction = 'ltr' | 'rtl';

export type TableRow = {agentName: string, agentExten: string, state: string, agentNo: string, stateTime: string,
  releaseCode: string, aCDCalls: string, nonACDCalls: string, dNIS: string, aN: string};

@Component({
  selector: 'storybook-login-form',
  templateUrl: './login-form.component.html',
  styleUrls: ['./login-form.scss'],
  imports: [
    TranslatePipe,
    ReactiveFormsModule,
    StoryInputComponent,
    NgIf,
    ButtonContinueComponent,
    CardComponent
  ],
  standalone: true
})
export default class LoginFormComponent implements OnInit {
  ACC_VERSION: string;


  // @Input() _translate?: TranslateService;


  extractDirection(): Direction {
    //if (this._translate)
    //  if (this._translate.currentDir)
    //    if (this._translate.currentDir == 'rtl')
    //      return 'rtl';
    return 'ltr'
  }


  @Input() tableObj!: TableRow[];
  //@Input() formService!: AuthService;

  @Input() isLoginFailed = false;

  @Input() loginErrorMessage = '';

  @Input() isLoading: boolean = false;

  @Input() Version: string = '';


  openReplacePassword() {
    //this.replacePassFormService.open(ReplacePassForm2Component);
  }


  public variableSource: BehaviorSubject<TableRow[]> = new BehaviorSubject<TableRow[]>(this.tableObj);
  public variable$ = this.variableSource.asObservable();

  updateTableSource(variable: TableRow[]) {
    this.setTableData(variable);
    this.variableSource.next(variable);
  }

  getTableData() {
    return this.tableObj;
  }

  setTableData(tableObj: TableRow[]) {
    this.tableObj = tableObj;
  }

  public exampleData: string =
    '[{"agentName":"Agent Name","agentExten":"Agent Exten.","state":"State","agentNo":"Agent No.","stateTime":"State Time","releaseCode":"Release Code","aCDCalls":"ACD Calls","nonACDCalls":"Non ACD Calls","dNIS":"DNIS","aN":"ANI"},' +
    '{"agentName":"Agent1","agentExten":"0","state":"Logout","agentNo":"1001","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
    '{"agentName":"Agent2","agentExten":"0","state":"Logout","agentNo":"1002","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
    '{"agentName":"Agent5","agentExten":"0","state":"Logout","agentNo":"1005","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
    '{"agentName":"Agent3","agentExten":"0","state":"Logout","agentNo":"1003","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
    '{"agentName":"Agent6","agentExten":"0","state":"Logout","agentNo":"1006","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"}]';





  /**
   * @ignore
   * Component property to define ordering of Inputs
   */
  storyInputsInOrder: StoryInput[] = [];

  @Input() mForm: FormGroup = new FormGroup({});

  @Input() isLoggedIn = false;

  // tslint:disable-next-line: no-output-on-prefix
  @Output() onPinInput = new EventEmitter<Event>();

  // tslint:disable-next-line: no-output-on-prefix
  @Output() onArchiveInput = new EventEmitter<any>();

  @Output() sendLoginReq = new EventEmitter();



  constructor() {
    this.setTableData(JSON.parse(this.exampleData));
  }

  @Input()
  set storyInputs(arr: StoryInput[]) {
    const initialTasks = [
      ...arr.filter(t => t.state === 'USER NAME'),
      ...arr.filter(t => t.state !== 'USER NAME'),
    ];
    const filteredTasks = initialTasks.filter(
      t => t.type === 'password' || t.state === 'USER NAME' || t.type === 'tel'
    );
    this.storyInputsInOrder = filteredTasks.filter(
      t => t.type === 'password' || t.state === 'USER NAME' || t.type === 'tel'
    );
  }

  onSubmit(): void {
    console.warn('Login Request!');

    this.sendLoginReq.emit();
  }

  ngOnInit(): void {
    this.updateTableSource(JSON.parse(this.exampleData));
  }


  get userName(): AbstractControl {
    return this.mForm?.get('username')!;
  }

  get email(): AbstractControl {
    return this.mForm?.get('email')!;
  }

  get password(): AbstractControl {
    return this.mForm?.get('password')!;
  }

  get phone(): AbstractControl {
    return this.mForm?.get('phone')!;
  }



}

