import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';
import {AppConfig} from "../app.config";




const AUTH_API = AppConfig.accServer.ACCWEBServers+'/Aeonix-App-Center/auth/';

const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

@Injectable({
  providedIn: 'root'
})
export class AuthService {
  constructor(private http: HttpClient) { }

  login(username: string, password: string): Observable<any> {
    return this.http.post(AUTH_API + 'signin', {
      username,
      password
    }, httpOptions);
  }

  registerForm(username: string, email: string, password: string, phone: string): Observable<any> {
    return this.http.post(AUTH_API + 'register-form', {
      username,
      email,
      password,
      phone,
    }, {responseType: 'text'});
  }

  replacePassForm(username: string, oldPassword: string, password: string, confirmPassword: string): Observable<any> {
    return this.http.post(AUTH_API + 'replace-pass-form', {
      username,
      oldPassword,
      password,
      confirmPassword,
    }, {responseType: 'text'});
  }

  register(username: string, email: string, password: string, roles: Array<String>): Observable<any> {
    return this.http.post(AUTH_API + 'signup', {
      username,
      email,
      password,
      roles,
    }, httpOptions);
  }

  getToken(url: string, username: string, email: string, password: string): Observable<any> {
    return this.http.post(url, {
      username,
      email,
      password
    }, httpOptions);
  }

  // login, register
  refreshToken(token: string, url?: string): Observable<any>  {
    return this.http.post((url ?? AUTH_API) + 'refreshtoken', {
      refreshToken: token
    }, httpOptions);
  }

  // open app in a new browser tab
  webapptab(token: string, webapp: string): Observable<any>  {
    return this.http.post(AUTH_API + 'webapptab', {
      refreshToken: token,
      webApp: webapp
    }, httpOptions);
  }

  getPassExpireDate(token: string): Observable<any> {
    return this.http.post(AUTH_API + 'passexpdate', {
      accessToken: token,
    }, httpOptions);
  }


  getPermittedWebAppList(token: string): Observable<any> {
    return this.http.post(AUTH_API + 'permitwebapplist', {
      accessToken: token,
    }, httpOptions);
  }

  getAccountDetails(username: string): Observable<any> {
    return this.http.post(AUTH_API + 'getaccountdetails', {
      username: username,
    }, httpOptions);
  }

  setAccountDetails( changedDetails: string): Observable<any> {
    return this.http.post(AUTH_API + 'setaccountdetails', {
      detail: changedDetails
    }, httpOptions);
  }

  getConfigurationData(): Observable<any> {
    return this.http.get(AUTH_API + 'getconfigurationdata', httpOptions);
  }

  setConfigurationData( changedProperties: string): Observable<any> {
    return this.http.post(AUTH_API + 'setconfigurationdata', {
      prop: changedProperties
    }, httpOptions);
  }

  resetConfigurationData(): Observable<any> {
    return this.http.get(AUTH_API + 'resetconfigurationdata', httpOptions);
  }

  TSV_ValidateCodeByName(username: string, email: string, code: string): Observable<any> {
    return this.http.post(AUTH_API + 'tsv_codevalidatebyname', {
      username,
      email,
      code,
    }, httpOptions);
  }

  TSV_ReplacePassForm(username: string, oldPassword: string, password: string,
                      confirmPassword: string, pinCodeToken: string): Observable<any> {
    return this.http.post(AUTH_API + 'tsv_replace-pass-form', {
      username,
      oldPassword,
      password,
      confirmPassword,
      pinCodeToken,
    }, httpOptions);
  }


  ResetPassByMail( username: string, email: string): Observable<any> {
    return this.http.post(AUTH_API + 'forgotpassword', {
      username,
      email,
    }, httpOptions);
  }

  TSV_GenerateCodeByName( username: string, email: string): Observable<any> {
    return this.http.post(AUTH_API + 'tsv_codegeneratebyname', {
      username,
      email,
    }, httpOptions);
  }

  TSV_GenerateCodeByEmail( username: string, email: string): Observable<any> {
    return this.http.post(AUTH_API + 'tsv_codegeneratebyemail', {
      username,
      email,
    }, httpOptions);
  }

  logout(refreshToken: string): Observable<any> {
    return this.http.post(AUTH_API + 'signout', {
      refreshToken,
    }, httpOptions);
  }

  validateAuthentication(): Observable<any> {
    return this.http.get(AUTH_API + 'validate-authentication', httpOptions);
  }
}
