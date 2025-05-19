import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import {concat, EMPTY, iif, Observable, of} from 'rxjs'
import {TokenStorageService} from "./token-storage.service";
import {AppConfig} from "../app.config";
import {catchError, map} from "rxjs/operators";


const AUTH_API = AppConfig.accServer.ACCWEBServers + '/Aeonix-App-Center/auth/';
const API_URL = AppConfig.accServer.ACCWEBServers + '/accDashboard/auth/';


const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};

const withCredentials = {
  headers: new HttpHeaders({ 'withCredentials': 'true' })
};

@Injectable({
  providedIn: 'root'
})
export class AuthService {
  constructor(private http: HttpClient, public tokenService: TokenStorageService) { }

  public logout(isAeonixAppCenterOn?: boolean ): Observable<void> {
    let refreshToken = (isAeonixAppCenterOn ? this.tokenService.getRefreshToken() : null);

    return concat(
        this.sendLogOutMsgToAdminBackend().pipe(catchError(() => of({}))),
        iif( () => isAeonixAppCenterOn ,
            this.sendLogOutMsgToAppCenter(refreshToken).pipe(catchError(() => of({}))),
            EMPTY
        ),
        of("USER LOGGED OUT").pipe(map(x => console.log(x))),
    );
  }

  login(username: string, password: string, lang: string): Observable<any> {
    return this.http.post(AUTH_API + 'signin', {
      username,
      password,
      lang
    }, httpOptions);
  }



  private sendLogOutMsgToAppCenter(token: string): Observable<any> {
    return this.http.post(AUTH_API + 'signout', {
      refreshToken: token,
    }, httpOptions);
  }

  private sendLogOutMsgToAdminBackend(): Observable<any> {
    return this.http.post(API_URL + 'signout', {
    }, httpOptions);
  }

  signin(username: string, password: string, lang: string): Observable<any> {
    return this.http.post(API_URL + 'signin', {
      username,
      password,
      lang
    }, httpOptions);
  }

  getAllProfileFieldsLogger(): Observable<any> {
    return this.http.get(API_URL + 'allProfileFields/Logger', httpOptions);
  }


  private sendLogOutMsg(token: string): Observable<any> {
    return this.http.post(AUTH_API + 'signout', {
      refreshToken: token,
    }, httpOptions);
  }

  // login, register
  refreshToken(token: string) {
    return this.http.post(AUTH_API + 'refreshtoken', {
      refreshToken: token
    }, httpOptions);
  }

  isAeonixAppCenterOn(): Observable<any> {
    return this.http.get( API_URL + 'aeonix-app-center-status', withCredentials);
  }
}
