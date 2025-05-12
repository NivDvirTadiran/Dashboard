import {Component, ElementRef, Input, OnDestroy, OnInit, Renderer2, ViewChild} from '@angular/core';
import {StoryInput} from "./story-input.model";
import {AuthService} from "../../_services/auth.service";
import {AbstractControl, FormControl, FormGroup, Validators} from "@angular/forms";
import {TokenStorageService} from "../../_services/token-storage.service";
import {ActivatedRoute, Router} from "@angular/router";
import {UserService} from "../../_services/user.service";
import {Subscription} from "rxjs";
import {AppConfig} from "../../app.config";
import LoginService from "./login.service";
import {DashboardService, TableRow} from '../../dashboard/dashboard.service';
import {environment} from "../../../environments/environment";
import {Login2Module} from "../login2.module";


// parse full URL to ip,port, params
const parsedUrl = new URL(window.location.href);
const url = parsedUrl.hostname;
const port = parsedUrl.port;
const protocol = parsedUrl.protocol;
const AUTH_DASHBOARD_API = protocol + "//" + url + ":" +  port
  + '/accDashboard/auth';


export interface DialogData {
  username: string;
  password: string;
}


export interface Login {
  WS: "";
  lang: "en";
  user: string;
  pass: string;
}

@Component({
  selector: 'login-main',
  templateUrl: './login-main.component.html',
  styleUrls: ['./login-main.component.scss'],
  standalone: false
})
export class LoginMainComponent implements OnInit, OnDestroy {

  public isLoggedIn = false;
  isLoginFailed = false;
  loginErrorMessage = '';
  roles: string[] = [];
  public isLoading = false;
  eventBusSubLoginMain?: Subscription;


  WS: "";
  lang: "en";
  user: string;
  pass: string;
  //url: string = 'accDashboard/login';
  url: string = 'auth/login'
  private ws_list: string;
  private font: string;
  private dp_app_center: string;









  constructor(private renderer: Renderer2,
              public authService: AuthService,
              public userService: UserService,
              private tokenStorage: TokenStorageService,
              private router: Router,
              private activatedRoute:ActivatedRoute,
              private loginService: LoginService) {
    this.loginForm = new FormGroup({
      username: new FormControl('ea', Validators.minLength(2)),
      password: new FormControl('e1996', Validators.minLength(2)),
    });
    this.ws_list = localStorage.getItem('ws_list') || '';
    this.font = localStorage.getItem('ws_dynamicFrontSize') || '';
    //this.lang = localStorage.getItem('ws_Accept-Language') || '';
    this.dp_app_center = sessionStorage.getItem('DP_APP_CENTER') || '';

  }


  /**
   * Is this the principal call to action on the login-main?
   */
  storyInputsInOrder: StoryInput[]  = [
    { /*...mStoryInput.Default.args?.['storyInput'],*/ id: '1', title: 'username', state: 'USER NAME', icon: './stories/assets/icons/User2ldpi.svg', type: 'text', placeholder: 'Ex.Saul Ramirez' , hide: false },
    { /*...mStoryInput.Default.args?.['storyInput'],*/ id: '2', title: 'password', state: 'PASSWORD', icon: './stories/assets/icons/LockIcon2ldpi.svg', type: 'password', placeholder: 'your_password', hide: false   },
  ];

  /**
   * Is this the principal call to action on the login-main?
   */
  @Input()
  primary = true;

  /**
   * What background color to use
   */
  @Input()
  backgroundColor?: string;


  /**
   * What background color to use
   */
  @Input()
  background?: string;

  /**
   * Button contents
   *
   * @required
   */
  @Input()
  label = 'Login2Component';


  loginForm: FormGroup;

  public get classes(): string[] {
    const mode = this.primary ? 'storybook-page2--primary' : 'storybook-page2--secondary';

    return ['storybook-page2', mode];
  }

  @ViewChild('formHeader', { static: false }) mainHeader?: ElementRef;


  public ACC_VERSION: string = 'ACC_VERSION';

  ngOnInit(): void {

    this.userService.getAccVersion().subscribe(
      data => { this.ACC_VERSION = data; AppConfig.webAppVersion = data; },
      err => { this.ACC_VERSION = JSON.parse(err.error).message; }
    );

  };


  get getUsernameCurrentFieldValue(): AbstractControl {
    return this.loginForm.get('username')?.value;
  }

  private setUsernameCurrentFieldValue(name: string) {
    this.loginForm.get('username')?.setValue(name);
  }

  get getPasswordCurrentFieldValue(): AbstractControl {
    return this.loginForm.get('password')?.value;
  }

  private setPasswordCurrentFieldValue(pass: string) {
    this.loginForm.get('password')?.setValue(pass);
  }


  onSubmit(): void {
    const { username, password, lang } = this.loginForm.value;
    console.log("Performing auto signin to accDashboard");

    this.loginService.login(username, password, lang)
      .then((data) =>  {
        console.log("data: " + data.toString())
        if (data) this.gsOmniDashboard();
      })
  }

  reloadPage(): void {
    this.router.navigate(['/login-main']).then(() => {window.location.reload()});
  }

  dashboardPage(): void { this.router.navigate(['/dashboard'], {relativeTo: this.activatedRoute}); }
  gridstackDashboard(): void { this.router.navigate(['/dashboard/gridstack-dashboard'], {relativeTo: this.activatedRoute}); }
  gsOmniDashboard(): void { this.router.navigate(['/dashboard/gs-omni-dashboard'], {relativeTo: this.activatedRoute}); }


  changLanguage(event: Event) {
    //
  }


  ngOnDestroy(): void {
    if (this.eventBusSubLoginMain)
      this.eventBusSubLoginMain.unsubscribe();
  }

}

