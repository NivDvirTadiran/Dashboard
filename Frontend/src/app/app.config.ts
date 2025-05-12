import {ApplicationConfig, InjectionToken} from '@angular/core';
import { provideRouter } from '@angular/router';
import { provideHttpClient } from '@angular/common/http';
import { routes } from './app.module';


// parse ful url to ip,port, params
var parsedUrl = new URL(window.location.href);
var url = parsedUrl.hostname;
var port = parsedUrl.port;
var protocol = parsedUrl.protocol;
const OMNI_URL_PREFIX = protocol + "//" + url + ":" + port;


export const AppConfig = {
  accServer: {
    ACCWEBServers: OMNI_URL_PREFIX,
    ACCServerAddress1: "",
    ACCServerAddress2: ""
  },
  routes: {
    heroes: 'heroes',
    error404: '404',
    agentlogon: 'agent-logon',
    omnilogon: 'omni-logon',
    accomnipage: 'acc-omni-page',
    mysessions: 'my-sessions',
  },
  endpoints: {
    heroes: 'https://nodejs-example-app.herokuapp.com/heroes',
    accWS: 'http://localhost://blabla/',
    accBaseP:  '/accagentapi',
    accLoginP: '/agentlogon',
    accLoginPJWT: '/agentlogongate',
    accRequestP: '/agentrequest',
    accNotificationP: '/accnotification',
    SSORequestP: '/accssoenable',
    accSse:  '/accagentapi/sse',
    accSseSubscribeP:  '/accagentapi/ssesubscribe',
    accWSPort: '8445'
    //accWSPort: '8443'
  },
  keys: {
    TOKEN_KEY: 'token',
    REFRESHTOKEN_KEY: 'auth-refreshtoken',
    USER_KEY: 'user',
    SIGNOUT_MARK: 'user-logout',
    SESSION_ID: 'session-id'
  },
  translateLanguage: {
    lang: 'eng',
    dir: 'ltr'
  },
  sessionId: '',
  votesLimit: 3,
  snackBarDuration: 3000,
  repositoryURL: 'https://github.com/Ismaestro/angular5-example-app',
  appName: 'accOMNI',
  webAppVersion: "8.06.xxx",
  isAeonixAppCenterOn: false
};
