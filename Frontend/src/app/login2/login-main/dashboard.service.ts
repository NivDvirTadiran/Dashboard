import {Injectable, OnDestroy} from '@angular/core';
import {HttpClient, HttpHeaders} from "@angular/common/http";
import { BehaviorSubject } from 'rxjs';
import {environment} from "../../../environments/environment";
import LoginService from "./login.service";
import {AppConfig} from "../../app.config";

export type TableRowNew = {agentName: string, agentExten: string, state: string, agentNo: string, stateTime: string,
                releaseCode: string, aCDCalls: string, nonACDCalls: string, dNIS: string, aN: string};

export type TableRow = {AgentId: string,AgentName: string,AgentNo: string,AgentExten: string,State: string,StateTime: string,ACDCalls: string,OACDCalls: string,ChatContacts: 0,EmailContacts: 0,ACDCallsAnswered: 0,OACDCallsAnswered: string,ChatContactsAnswered: string,EmailContactsAnswered: string,TotalACDTalkTime: string,HeldCalls: string,LongestOnHeldTime: string,TotalHeldCalls: string,TotalHeldTime: string,Conference: 0,TimeInConference: 0,TotalTxfrOutCalls: string,NONACCalls: string,TotalIncNONACDCalls: string,TotalOutNONACDCalls: string,TotalNONACDTalkTime: string,TotalReleaseTime: string,DNIS: string,ANI: string,CurrentACDGroup: string };

// parse full URL to ip,port, params
const parsedUrl = new URL(window.location.href);
const url = parsedUrl.hostname;
const port = parsedUrl.port;
const protocol = parsedUrl.protocol;

const AUTH_DASHBOARD_API = protocol + "//" + url + ":" + port + '/accDashboard/auth';



function isHTMLTableElement(elem: HTMLTableElement | HTMLElement) : elem is HTMLTableElement {
    return elem.tagName === 'TABLE';
}


@Injectable({
  providedIn: 'root'
})
export class DashboardService implements OnDestroy {


    tableObj: TableRow[] = [];
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

    public exampleDataNew: string =
        '[{"agentName":"Agent Name","agentExten":"Agent Exten.","state":"State","agentNo":"Agent No.","stateTime":"State Time","releaseCode":"Release Code","aCDCalls":"ACD Calls","nonACDCalls":"Non ACD Calls","dNIS":"DNIS","aN":"ANI"},' +
        '{"agentName":"Agent1","agentExten":"0","state":"Logout","agentNo":"1001","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
        '{"agentName":"Agent2","agentExten":"0","state":"Logout","agentNo":"1002","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
        '{"agentName":"Agent5","agentExten":"0","state":"Logout","agentNo":"1005","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
        '{"agentName":"Agent3","agentExten":"0","state":"Logout","agentNo":"1003","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"},' +
        '{"agentName":"Agent6","agentExten":"0","state":"Logout","agentNo":"1006","stateTime":"0:00","releaseCode":"","aCDCalls":"0","nonACDCalls":"0","dNIS":"0","aN":"0"}]';


  public exampleData: string =
    '[' +
    '{"AgentId": "555","AgentName": "Agent6","AgentNo": "1006","State": "Logout","StateTime": "0","ACDCalls": "0","OACDCalls": "0","ChatContacts": "0","EmailContacts": "0","ACDCallsAnswered": "0","OACDCallsAnswered": "0","ChatContactsAnswered": "0","EmailContactsAnswered": "0"}' +
    //'{Agent Id: 2,Agent Name: Agent2,Agent No: 1002,Agent Exten.: 3510,State: Idle,State Time: 8644,OMNI State: Idle,OMNI State Time: 8644,Release Code: ,ACD Calls: 0,O.ACD Calls: 0,Chat Contacts: 0,Email Contacts: 0,ACD Calls Answered: 0,O.ACD Calls Answered: 0,Chat Contacts Answered: 0,Email Contacts Answered: 0,Total ACD Talk Time: 0,Held Calls: 0,Longest On Held Time: 0,Total Held Calls: 0,Total Held Time: 0,In Conference: 0,Time In Conference: 0,Total Txfr Out Calls: 0,NON-ACD Calls: 0,Total Inc NON-ACD Calls: 0,Total Out NON-ACD Calls: 0,Total NON-ACD Talk Time: 0,Total Release Time: 0,DNIS: ,ANI: ,Current ACD Group:  }' +
   // '{Agent Id: 4,Agent Name: Agent4,Agent No: 1004,Agent Exten.: ,State: Logout,State Time: 0,ACD Calls: 0,O.ACD Calls: 0,Chat Contacts: 0,Email Contacts: 0,ACD Calls Answered: 0,O.ACD Calls Answered: 0,Chat Contacts Answered: 0,Email Contacts Answered: 0,Total ACD Talk Time: 0,Held Calls: 0,Longest On Held Time: 0,Total Held Calls: 0,Total Held Time: 0,Conference: 0,Time In Conference: 0,Total Txfr Out Calls: 0,NON-ACD Calls: 0,Total Inc NON-ACD Calls: 0,Total Out NON-ACD Calls: 0,Total NON-ACD Talk Time: 0,Total Release Time: 0,DNIS: ,ANI: ,Current ACD Group:  }' +
   // '{Agent Id: 555,Agent Name: Agent6,Agent No: 1006,Agent Exten.: ,State: Logout,State Time: 0,ACD Calls: 0,O.ACD Calls: 0,Chat Contacts: 0,Email Contacts: 0,ACD Calls Answered: 0,O.ACD Calls Answered: 0,Chat Contacts Answered: 0,Email Contacts Answered: 0,Total ACD Talk Time: 0,Held Calls: 0,Longest On Held Time: 0,Total Held Calls: 0,Total Held Time: 0,Conference: 0,Time In Conference: 0,Total Txfr Out Calls: 0,NON-ACD Calls: 0,Total Inc NON-ACD Calls: 0,Total Out NON-ACD Calls: 0,Total NON-ACD Talk Time: 0,Total Release Time: 0,DNIS: ,ANI: ,Current ACD Group:  }' +
    ']';



  timer: any;
    errors = 0;
    lastPing = '';
    pathToGo = '';
    isPaused: boolean = null;
    isWSChanged = false;
    //tslint:turn-off-next-line:variable-name
    ws_list = '';
    font = '';
    lang = '';
    dp_app_center = 'DP_ENABLE';
    Request_TIMEOUT = 3000;
    pingXmlHttpReq: any;
    xmlHttpReq: any;
    table: HTMLElement;
    headers: any;

    constructor(private http: HttpClient,
                private loginService: LoginService) {
        console.log('WidgetTableService.constructor');
        this.ws_list = localStorage.getItem('ws_list') || '';
        this.font = localStorage.getItem('ws_dynamicFrontSize') || '';
        this.lang = localStorage.getItem('ws_Accept-Language') || '';
        this.dp_app_center = sessionStorage.getItem('DP_APP_CENTER') || '';
        //setInterval(()=> { this.runUpdateTimer() }, this.Request_TIMEOUT);
        this.updateTableSource(JSON.parse(this.exampleData));

        this.startPollingData();
    }

  async loginDashboard(): Promise<void> {
    console.warn('Login Request from login-OMNI_DASHBOARD!');
    const { username, password } = { username: this.loginService.username, password: this.loginService.password };

    console.log(username);
    console.log(password);
    console.log(AUTH_DASHBOARD_API + '/login');

    console.log('requestUpdate');

    if (this.xmlHttpReq) {
      return new Promise((resolve, reject) => {
        this.xmlHttpReq.open('POST', AUTH_DASHBOARD_API + '/login', true);
        this.xmlHttpReq.setRequestHeader('Access-Control-Allow-Origin', '*');
        this.xmlHttpReq.setRequestHeader('Content-Type', 'application/json');
        this.xmlHttpReq.onreadystatechange = () => {
          if (this.xmlHttpReq.readyState === XMLHttpRequest.DONE) {
            if (this.xmlHttpReq.status === 200) {
              console.log("login success");
              this.loginService.isUserLoggedIn = true;
              this.loginService.loginErrorMessage = "login success";
              resolve();
            } else if (this.xmlHttpReq.status === 401) {
              console.log("unauthorized");
              this.loginService.isUserLoggedIn = false;
              this.loginService.loginErrorMessage = "unauthorized";
              reject(new Error('Unauthorized'));
            } else {
              console.log("login failed");
              reject(new Error('Login failed'));
            }
          }
        };
        this.xmlHttpReq.send();
      });
    }
    return Promise.reject(new Error('XMLHttpRequest not initialized'));
  }

    startPollingData() {
        console.log('WidgetTableService.ngOnInit');
        this.initRequest();
        this.runUpdateTimer();
    }

    initRequest() {
        console.log('initRequest');
        this.pingXmlHttpReq = new XMLHttpRequest();
        this.xmlHttpReq = new XMLHttpRequest();
      //      this.pingXmlHttpReq.open('GET', path, true);
      //      this.pingXmlHttpReq.onload = () => {
      //        if (this.pingXmlHttpReq.readyState === 4 && this.pingXmlHttpReq.status === 200) {
      //          this.xmlHttpReq = this.pingXmlHttpReq;
      //        }
      //      };
      //      this.pingXmlHttpReq.send();
    }


    requestUpdate() {
        console.log('requestUpdate');
        if (this.xmlHttpReq) {
            this.xmlHttpReq.open('GET', AUTH_DASHBOARD_API+ '/Agents.Brief_Report.nlat?agentId=1,2', true);
            this.xmlHttpReq.setRequestHeader('RefreshMode', 'true');
            this.xmlHttpReq.setRequestHeader('Content-Type', 'application/json');
            this.xmlHttpReq.setRequestHeader("Authorization", "Bearer " + sessionStorage.getItem("token"));

            this.xmlHttpReq.onreadystatechange = () => {
                if (this.xmlHttpReq.readyState === XMLHttpRequest.DONE)
                {
                    if (this.xmlHttpReq.status == 200)
                    {
                        //alert(self.xmlHttpReq.responseText);
                        this.cacheElement.innerHTML=this.xmlHttpReq.responseText;

                        const jsonStr = this.xmlHttpReq.responseText.replaceAll("-", '');

                      this.updateTableSource(JSON.parse(jsonStr));

                        this.runUpdateTimer();
                        this.errors=0;

                        var ServerTime = this.xmlHttpReq.getResponseHeader("accServerTime");

                    }
                    else if (this.xmlHttpReq.status == 401 )
                    {
                        this.runUpdateTimer(this.Request_TIMEOUT);

                        if ( this.isRefreshing() == false )
                        {
                            this.refreshToken(false);

                        }
                        else
                        {
                            console.log( "Token is expired and refresh process is already on");
                        }
                    }
                    else
                    {
                        this.errors++;
                        this.runUpdateTimer(this.Request_TIMEOUT*5);
                    }
                }
            };
            this.xmlHttpReq.setRequestHeader('Authorization', 'Bearer ' + sessionStorage.getItem("token"));
            this.xmlHttpReq.send();
        }
    }

    requestUpdatePing(url: string) {
        if (this.pingXmlHttpReq) {
            this.http.get(url, {observe: 'response'}).subscribe(response => {
                this.doPingContextUpdate(response);
            });
        }
    }

    cacheElement= document.createElement("span");
    doContextUpdate() {
        console.log("doContextUpdate: this.xmlHttpReq.readyState - " + this.xmlHttpReq.readyState);
        if (this.xmlHttpReq.readyState === XMLHttpRequest.DONE)

        {
            if (this.xmlHttpReq.status == 200)
            {
                //alert(self.xmlHttpReq.responseText);
                this.cacheElement.innerHTML=this.xmlHttpReq.responseText;
                console.log("doContextUpdate: table - " + this.xmlHttpReq.responseText);
                var table = this.xmlHttpReq.responseText.tableToJSON();
                console.log("doContextUpdate: table - " + table);
                let x=this.cacheElement.getElementsByTagName("onRefresh");
                //alert(x.length);
                //console.log("doContextUpdate: cacheElement - " + cacheElement + ", x - " + x + ", length - " + x.length);
                for(var i=0; i< x.length ; i++) {
                    //console.log("doContextUpdate: eval(x[i].getAttribute(\"event\") - " + eval(x[i].getAttribute("event"));
                    //alert(x[i].getAttribute("event")+"(cacheElement)");
                    const eventFunction = new Function('cacheElement', x[i].getAttribute("event") + "(cacheElement)");
                    eventFunction(this.cacheElement);
                }
                this.runUpdateTimer();
                this.errors=0;

                //10-Aug-2022 YR BZ#56641
                //var today = new Date();
                //var time = today.getHours() + ":" ted+ today.getMinutes() + ":" + today.getSeconds();
                var ServerTime = this.xmlHttpReq.getResponseHeader("accServerTime");



                var spanServerTime = this.titleOrTiltle_md().document.getElementById("accServerTime");
                //console.log("accServerTime: " + ServerTime + ", Local: " + time);.contains("Period")
                if((spanServerTime != null) && (ServerTime != null) && (spanServerTime.innerHTML != ServerTime)){
                    //spanServerTime.innerHTML = time;
                    console.log("Updating accServerTime - " + ServerTime);
                    spanServerTime.innerHTML = ServerTime;
                    //spanServerTime.innerHTML = self.xmlHttpReq.getResponseHeader("accServerTime");

                    //var spanServerTime = parent.parent.title.document.createElement('div');
                    //spanServerTime.innerHTML = self.xmlHttpReq.getResponseHeader("accServerTime");
                    //parent.parent.title.document.getElementById("accServerTime").appendChild(spanServerTime);
                }
            }
            else if (this.xmlHttpReq.status == 401 )
            {
                this.runUpdateTimer(this.Request_TIMEOUT);

                if ( this.isRefreshing() == false )
                {
                    this.refreshToken(false);

                }
                else
                {
                    console.log( "Token is expired and refresh process is already on");
                }
            }
            else
            {
                this.errors++;
                this.runUpdateTimer(this.Request_TIMEOUT*5);
            }
        }
    }

    titleOrTiltle_md() {
        const currentTitle = parent.parent.document
            .getElementById("main-page").getElementsByTagName("frame")[0].title;
        var Obj: any = null;
        if (currentTitle == "title")
        {
            Obj = parent.parent.document.getElementsByTagName('title');
        }
        if (currentTitle == "title_md")
        {
            Obj = parent.parent.document.getElementsByTagName('title_md');
        }
        return Obj;
    }


    doPingContextUpdate(response: any) {
        if (response.status === 200) {
            window.location.assign(this.pathToGo + this.getParams());
        } else if (response.status === 401) {
            this.errors++;
            this.runUpdateTimer(this.Request_TIMEOUT * 5);
            if (!this.isRefreshing()) {
                if (this.refreshToken(true)) {
                    window.location.assign(this.pathToGo + this.getParams());
                }
            }
        } else {
            this.errors = 8;
            this.isPaused = null;
            this.runUpdateTimer();
        }
    }

    refreshToken(toClusterIp: boolean): boolean {
        this.showRefresh();
        console.log('Starting Token refresh process.');
        const data = JSON.stringify({
            refreshToken: sessionStorage.getItem('auth-refreshtoken') || ''
        });

        const myUrl = window.location.href;
        const myIp = myUrl.split('//')[1].split(':')[0];
        let clusterIp: string;
        const [ws1, ws2] = this.ws_list.split(';');
        const ws1Ip = this.getIp(ws1);
        const ws2Ip = this.getIp(ws2);
        clusterIp = myIp === ws1Ip && ws2Ip ? ws2Ip : ws1Ip;
        const myHost = toClusterIp ? clusterIp : myIp;
        const myPort = myUrl.split(':')[2].split('/')[0];
        this.http
            .post(
                `https://${myHost}:${myPort}/Aeonix-App-Center/auth/refreshtoken`,
                data,
                {
                    headers: new HttpHeaders({
                        'Content-Type': 'application/json',
                        Authorization: `Bearer ${sessionStorage.getItem('token') || ''}`,
                        'Access-Control-Allow-Origin': '*'
                    })
                }
            )
            .subscribe({
                    next: (response: any) => {
                        sessionStorage.setItem('auth-refreshtoken', response.refreshToken.toString());
                        sessionStorage.setItem('token', response.accessToken.toString());
                        document.cookie = `Authorization=${sessionStorage.getItem('token')}; path=/accDashboard;`;
                        if (response.accessToken.toString() !== null) { this.requestUpdate(); }
                        this.closeRefresh();
                        return true;
                    },
                    error: (e) => {
                        console.log('Token refresh process failed!');
                        this.closeRefresh();
                        return false;
                    },
                    complete: () => {
                        console.log('Token refresh process completed!');
                    }
            });

        return false;
    }

    getParams(): string {
        let a1: string;
        if (this.dp_app_center === 'DP_ENABLE') {
            const userName = `GateUser: ${JSON.parse(sessionStorage.getItem('user') as any).username}`;
            const pass = sessionStorage.getItem('token') || '';
            const refreshtoken = sessionStorage.getItem('auth-refreshtoken') || '';
            a1 = `${userName},${pass},${refreshtoken}`;
        } else {
            const userName = sessionStorage.getItem('userName') || '';
            const pass = sessionStorage.getItem('pass') || '';
            a1 = `${userName},${pass}`;
        }
        return `?params=${btoa(a1)}`;
    }

    getIp(url: string): string | null {
        return url && url !== 'null' ? url.split('//')[1].split(':')[0] : null;
    }

    runUpdateTimer(delay: number = this.Request_TIMEOUT) {
        if (this.errors === 5) {
            this.showAlert();
        }

        if (this.errors < 10) {
            this.timer = setTimeout(() => this.requestUpdate(), delay);
        } else {
            console.log('ajaxRefresh::runUpdateTimer() - errors ' + this.errors);
            if (this.isPaused === null) {
                this.isPaused = true;
                this.isWSChanged = false;
                this.showAlert();
                const [ws1, ws2] = this.ws_list.split(';');
                const myUrl = window.location.href;
                const myIp = myUrl.split('//')[1].split(':')[0];
                const ws1Ip = this.getIp(ws1);
                const ws2Ip = this.getIp(ws2);

                const restUrl = '/accDashboard/login.jsp';

                if (myIp === ws1Ip && ws2 !== null) {
                    if (ws2 === this.lastPing) {
                        this.lastPing = ws1;
                        this.pathToGo = `${ws1}${restUrl}`;
                        this.requestUpdatePing(`${ws1}/accAgent/ping`);
                    } else {
                        this.lastPing = ws2;
                        this.pathToGo = `${ws2}${restUrl}`;
                        this.requestUpdatePing(`${ws2}/accAgent/ping`);
                    }
                } else {
                    this.pathToGo = `${ws1}${restUrl}`;
                    this.requestUpdatePing(`${ws1}/accAgent/ping`);
                }
            }
        }
    }

    showAlert() {
        console.log('ajaxRefresh:showAlert - connectionErr');
        const spanErr = document.getElementById('connectionErr');
        if (spanErr) {
            spanErr.style.color = 'red';
            spanErr.innerHTML = 'Connection Lost - Trying to Reconnect';
        }
    }

    closeAlert() {
        console.log('ajaxRefresh:closeAlert - connectionErr');
        const spanErr = document.getElementById('connectionErr');
        if (spanErr) {
            spanErr.innerHTML = '';
        }
    }

    showRefresh() {
        console.log('ajaxRefresh:showRefresh - connectionErr');
        const spanErr = document.getElementById('connectionErr');
        if (spanErr) {
            spanErr.innerHTML = 'Refreshing..';
        }
    }

    closeRefresh() {
        console.log('ajaxRefresh:closeRefresh - connectionErr');
        const spanErr = document.getElementById('connectionErr');
        if (spanErr) {
            spanErr.innerHTML = '';
        }
    }

    private isRefreshing() {
        return false;
    }

    ngOnDestroy(): void {
        this.xmlHttpReq.destroy();
        this.pingXmlHttpReq.destroy();
        clearTimeout(this.timer);
    }

}
