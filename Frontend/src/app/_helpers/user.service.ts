import { Injectable } from '@angular/core';
import {HttpClient, HttpHeaders} from '@angular/common/http';
import { Observable } from 'rxjs';
import {AppConfig} from "../app.config";


const API_URL = AppConfig.accServer.ACCWEBServers+'/accOMNI/auth/';

const httpOptions = {
  headers: new HttpHeaders({ 'Content-Type': 'application/json' })
};


@Injectable({
  providedIn: 'root'
})
export class UserService {
  constructor(private http: HttpClient) { }

  getPublicContent(): Observable<any> {
    return this.http.get(API_URL + 'all', { responseType: 'text' });
  }

  getAccVersion(): Observable<any> {
    return this.http.get(API_URL + 'acc-version', { responseType: 'text' });
  }

  getClusterAddress(): Observable<any> {
    return this.http.get(API_URL + 'clusteraddress', httpOptions);
  }

  isTowStepVerRequired(): Observable<any> {
    return this.http.get(API_URL + 'istsvon', httpOptions);
  }

  setLanguage(lang: string, username: string): Observable<any> {
    return this.http.post(API_URL + 'setlang',{
        lang,
        username}
      , httpOptions);
  }

  getLanguage(username: string): Observable<any> {
    return this.http.post(API_URL + 'getlang',{
        username}
      , httpOptions);
  }

}
