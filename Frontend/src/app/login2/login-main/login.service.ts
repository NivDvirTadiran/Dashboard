import { TokenStorageService } from "../../_helpers/token-storage.service";
import { Router } from '@angular/router';
import { MatDialog } from '@angular/material/dialog';
import {Injectable} from "@angular/core";
import {AuthService} from "../../_helpers/auth.service";
import {AppConfig} from "../../app.config";


@Injectable({
    providedIn: 'root'
})
export default class LoginService {
    public username: string = "";
    public password: string = "";
    public agentNo: string = "";
    isUserLoggedIn: boolean;
    loginErrorMessage = '';


    constructor(public dialog: MatDialog, public authService: AuthService,
                private tokenStorage: TokenStorageService, private router: Router) {
        this.updateLoginStatus();
    }

    login(username: string, password: string, lang: string): Promise<any> {

        this.username = username;
        let promiseLogin = new Promise((resolve, reject) => {
            try {
                var s = this.authService.signin(username, password, lang).subscribe({
                    next: data => {
                        if (data) {
                            this.agentNo = data.agentNo;
                            /*this.tokenStorage.saveToken(data.accessToken);
                            this.tokenStorage.saveRefreshToken(data.refreshToken);
                            this.tokenStorage.saveUser(data);
                            this.userName = this.tokenStorage.getUser().username;
                            this.isUserLoggedIn = true;*/
                            resolve(data);
                        } else {
                            this.isUserLoggedIn = false;
                            reject({error:{ message: "unknown" }})
                        }
                        s.unsubscribe();
                    },
                    error: err => {
                        switch (err.error?.message) {
                            case "credentials":
                                break;
                            default:
                                this.loginErrorMessage = err.error.message;
                        }
                        this.isUserLoggedIn = false;
                        reject(err)
                        s.unsubscribe();
                    }
                });
            } catch (e) {
                console.log("login request time over.");
                reject(e)
            }

        });
        return promiseLogin;
    }


    logout(): void {

        this.authService.logout(AppConfig.isAeonixAppCenterOn).subscribe({
            next: () => {
                this.username = null;
                this.isUserLoggedIn = false;
            },
            complete: () => {
                this.tokenStorage.signOut();
                this.router.navigate(['/']);
            }
        })

    }

    private updateLoginStatus() {
        this.username = this.tokenStorage.getUser()?.username;
        this.isUserLoggedIn = this.username != null;
    }

}
