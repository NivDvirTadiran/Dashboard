import { Pipe, PipeTransform } from '@angular/core';
import {TranslateService} from "./translate.service";
/*import {AppConfig} from "../../../app.config";
import {UserService} from "../../../_services/user.service";*/


@Pipe({
  name: 'translate',
  standalone: true,
  pure: false
})
export class TranslatePipe implements PipeTransform {

  constructor(private _translate: TranslateService) {}

  transform(value: string): any {
    if (!value) return;
    return this._translate.instant(value);
  }

}
