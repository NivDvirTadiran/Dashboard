import {AfterViewInit, Component, ElementRef, EventEmitter, Input, OnInit, Output, Renderer2, ViewChild} from '@angular/core';
import { StoryInput } from './story-input.model'
import {AbstractControl, FormControl, FormGroup, ReactiveFormsModule} from "@angular/forms";
import {PopoverOptions} from "./directive-input/popover.interface";
import {ActionInputComponent} from "./action-input/action-input.component";
import {ActionInput} from "./action-input/action-input.interface";

import {TranslateService} from "src/stories/pipes/translate/translate.service";
import {NgClass, NgIf, NgSwitch, NgSwitchCase} from "@angular/common";
import {TranslatePipe} from "../../../pipes/translate/translate.pipe";
import {MatIcon, MatIconModule} from "@angular/material/icon";
import {MatListItemIcon} from "@angular/material/list";


export interface GroupForm {
  username:  FormControl,
  password:  FormControl,
}

export interface ValidationForm {
  validationForm2: GroupForm,
}


@Component({
  selector: 'storybook-input',
  templateUrl: './story-input.component.html',
  standalone: true,
  imports: [
    ReactiveFormsModule,
    NgClass,
    TranslatePipe,
    NgSwitch,
    NgSwitchCase,
    NgIf,
    MatIcon,
    MatIconModule
  ],
  styleUrls: ['./story-inpu.component.scss']
})
export class StoryInputComponent implements OnInit, AfterViewInit{

  popover: PopoverOptions = {
    content: ActionInputComponent
  };

  @Input() storyInput?: StoryInput;

  @Output()
  onPinInput = new EventEmitter<Event>();

  // tslint:disable-next-line: no-output-on-prefix
  @Output()
  onArchiveInput = new EventEmitter<Event>();

  @Input() hideInput = false;

  @Input() toFocus: boolean = false;

  @Input() direction: 'rtl' | 'ltr' = 'ltr';

  constructor(public _translate: TranslateService,
              public renderer: Renderer2) {}

  showPassChange(): void {
    this.storyInput.type = (this.storyInput?.type == 'password' ? 'text' : 'password')
  }

  @Input() currentForm!: FormGroup;

  private isStrength: string = (this.storyInput?.state === 'NEW PASSWORD') ? 'storybook-input--addStrength' : 'storybook-input--clearStrength';

  public isPasswordTextHide: boolean = false;

  @Input() conditionList: string[] = [];

  @ViewChild('mInput', { static: false }) mInput?: ElementRef;

  @Input() state: string = 'edit';

  onStateChange(state: string) {
    if (state == 'edit' || state == 'gray') {this.state = state;}
    switch (state) {
      case 'gray':
        this.renderer.setAttribute(this.mInput?.nativeElement ,'readonly', 'true');
        this.renderer.setAttribute(this.mInput?.nativeElement ,'style', 'background-color: #ebf1f2;');
        break;
      case 'edit':
        this.renderer.removeAttribute(this.mInput?.nativeElement ,'readonly');
        this.renderer.removeAttribute(this.mInput?.nativeElement ,'style');
        break;
    }
  }

  get username(): AbstractControl {
    return this.currentForm.get('username')!;
  }

  get password(): AbstractControl {
    return this.currentForm.get('password')!;
  }

  public getErrorHeader(controllerType: string): string {
    let header: string = '';

    switch (controllerType) {
      case "phone":
        header = 'Must contain phone:';
        break;

      case "email":
        header = 'Must contain email:';
        break;

      case "oldPassword":
      case "confirmPassword":
      case "password":
        header = 'The password must contain:';
        break;

      default:
        header = controllerType;
    }

    return header;
  }

  public getErrorList(conditionList: String[]): ActionInput[] {

    let passConditions: string[] = [];


    var actionInputs: ActionInput[] = [];

    conditionList.forEach(condition => {
      switch (condition) {
        case "requiresMinLength":
          actionInputs.push({
            "conditionName": "requiresMinLengthValid",
            "presentingMessage": ' A minimum of 8 characters ',
            "isFulfilled":  !(this.requiresMinLengthValid || !(this.formControler.value.length > 0)),
          })
          break;

        case "requiresUppercase":
          actionInputs.push({
            "conditionName": "requiresUppercaseValid",
            "presentingMessage": ' At least 1 Uppercase letters',
            "isFulfilled": !(this.requiresUppercaseValid || !(this.formControler.value.length > 0)),
          })
          break;

        case "requiresLowercase":
          actionInputs.push({
            "conditionName": "requiresLowercaseValid",
            "presentingMessage": ' At least 1 lowercase letters',
            "isFulfilled": !(this.requiresLowercaseValid || !(this.formControler.value.length > 0)),
          })
          break;

        case "requiresDigit":
          actionInputs.push({
            "conditionName": "requiresDigitValid",
            "presentingMessage": ' A number',
            "isFulfilled": !(this.requiresDigitValid || !(this.formControler.value.length > 0))
          })
          break;

        case "requiresSpecialChars":
          actionInputs.push({
            "conditionName": "requiresSpecialCharsValid",
            "presentingMessage": ' At least 1 special character',
            "isFulfilled": !(this.requiresSpecialCharsValid || !(this.formControler.value.length > 0))
          })
          break;

        case "no2AdjacentIdenticalChars":
          actionInputs.push({
            "conditionName": "no2AdjacentIdenticalCharsValid",
            "presentingMessage": " No more than 2 adjacent identical characters",
            "isFulfilled": (this.no2AdjacentIdenticalCharsValid || !(this.formControler.value.length > 0))
          })
          break;

        case "requiresEnglishLettersOnly":
          actionInputs.push({
            "conditionName": "requiresEnglishLettersOnlyValid",
            "presentingMessage": " English letters only",
            "isFulfilled": (this.requiresEnglishLettersOnlyValid || !(this.formControler.value.length > 0))
          })
          break;

        case "requiresEmail":
          actionInputs.push({
            "conditionName": "requiresEmailValid",
            "presentingMessage": ' A well-formed email address',
            "isFulfilled": !(this.requiresEmailValid || !(this.formControler.value.length > 0))
          })
          break;

        case "requiresPhone":
          actionInputs.push({
            "conditionName": "requiresPhoneValid",
            "presentingMessage": ' A well-formed phone number',
            "isFulfilled": !(this.requiresPhoneValid || !(this.formControler.value.length > 0))
          })
          break;
      }
    });


    return actionInputs;
  }

  get passwordValid() {
    return this.formControler.errors === null;
  }

  get requiredValid() {
    return this.formControler.hasError("required");
  }

  get minLengthValid() {
    return this.formControler.hasError("minlength");
  }

  get requiresMinLengthValid() {
    return this.formControler.hasError("requiresMinLength");
  }

  get requiresDigitValid() {
    return this.formControler.hasError("requiresDigit");
  }

  get requiresUppercaseValid() {
    return this.formControler.hasError("requiresUppercase");
  }

  get requiresLowercaseValid() {
    return this.formControler.hasError("requiresLowercase") ;
  }

  get requiresSpecialCharsValid() {
    return this.formControler.hasError("requiresSpecialChars");
  }

  get no2AdjacentIdenticalCharsValid() {
    return this.formControler.hasError("no2AdjacentIdenticalChars");
  }

  get requiresEnglishLettersOnlyValid() {
    return this.formControler.hasError("requiresEnglishLettersOnly");
  }

  get requiresEmailValid() {
    return this.formControler.hasError("email");
  }

  get requiresPhoneValid() {
    return this.formControler.hasError("requiresPhoneChars");
  }

  get formControler(): AbstractControl {
    return this.currentForm.get(this.storyInput?.title.toString())!;
  }

  /**
   * Component method to trigger the onPin event
   * @param id string
   */
  onPin(id: any) {
    this.onPinInput.emit(id);
  }

  /**
   * Component method to trigger the onArchive event
   * @param id string
   */
  onArchive(id: any) {
    this.onArchiveInput.emit(id);
  }

  public get classes(): string[] {
    this.isStrength = (this.storyInput?.state === 'NEW PASSWORD') ? 'storybook-input--addStrength' : 'storybook-input--clearStrength';

    return [/*'storybook-input-strength', `storybook-input--${this.direction}`, this.isStrength*/];
  }

  ngOnInit(): void {
    this.isPasswordTextHide = this.storyInput?.state.includes('PASSWORD');
  }

  ngAfterViewInit(): void {
    this.onStateChange(this.state);
  }


}
