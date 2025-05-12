import { Injectable } from '@angular/core';

const TOKEN_KEY = 'token';
const REFRESHTOKEN_KEY = 'auth-refreshtoken';
const PINCODETOKEN_KEY = 'auth-pincodetoken';
const USER_KEY = 'user';

@Injectable({
  providedIn: 'root'
})
export class TokenStorageService {
  constructor() { }

  signOut(): void {
    window.sessionStorage.clear();
  }

  public saveToken(token: string): void {
    window.sessionStorage.removeItem(TOKEN_KEY);
    window.sessionStorage.setItem(TOKEN_KEY, token);
    const user = this.getUser();
    if (user.id) {
      this.saveUser({ ...user, accessToken: token });
    }
  }

  public getToken(): string | null {
    return window.sessionStorage.getItem(TOKEN_KEY);
  }

  public saveRefreshToken(token: string): void {
    window.sessionStorage.removeItem(REFRESHTOKEN_KEY);
    window.sessionStorage.setItem(REFRESHTOKEN_KEY, token);
    const user = this.getUser();
    if (user.id) {
      user.refreshToken = token;
      this.saveUser(user);
    }
  }

  public getRefreshToken(): string | null {
    return window.sessionStorage.getItem(REFRESHTOKEN_KEY);
  }

  public savePinCodeToken(token: string): void {
    window.sessionStorage.removeItem(PINCODETOKEN_KEY);
    window.sessionStorage.setItem(PINCODETOKEN_KEY, token);
  }

  public getPinCodeToken(): string | null {
    return window.sessionStorage.getItem(PINCODETOKEN_KEY);
  }

  public saveUser(user: any): void {
    window.sessionStorage.removeItem(USER_KEY);
    window.sessionStorage.setItem(USER_KEY, JSON.stringify(user));
  }

  public getUser(): any {
    const user = window.sessionStorage.getItem(USER_KEY);
    if (user) {
      return JSON.parse(user);
    }
    return {};
  }

  public getRoles(): any {
    const user = this.getUser();
    if (user.roles) {
      return user.roles;
    }
    return {};
  }

  public getUsername(): any{
    const user = this.getUser();
    return user.username ?? null;
  }

  public isSupervisorAdmin(): boolean {
    return this.getUser()?.roles?.includes("SupervisorAdmin");
  }
}
