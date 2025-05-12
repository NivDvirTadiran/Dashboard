package tadiran.webnla.bean;

import org.springframework.ws.WebServiceException;
import tadiran.emisweb.*;
import tadiran.webnla.NLAContext;
import tadiran.webnla.endpoint.EMISwebServiceClient;
import tadiran.webnla.servlet.NLAService;
//import tadiran.webnla.tag.LocaleMessage;
import java.net.MalformedURLException;
import java.net.URLDecoder;
//import java.net.URLEncoder;
import java.util.*;
import jakarta.servlet.ServletContext;
import jakarta.servlet.http.HttpSession;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;


/**
 * user's session data: work space, user's groups, user reports etc
 * @author evgeniv
 */
public class SessionData
{
    private static final Logger log;
    private static EMISwebPort wsEMIS_port;

    private static final int UPDATE_TIMEOUT=1000*60*1; //1 min
    private static int session_data_connected_server = NLAService.NOT_CONNECTED;
    private static Timer timer=new Timer("SessionData");
    private UpdaterTask ut;

    private static ServletContext ctx;
    private static NLAContext nlaCtx=NLAContext.getInstance();
    private static final int MaxAgentsInBriefReport=NLAContext.MaxAgentsInBriefReport;

    private RequestInfoHeaderType reqHeader=new RequestInfoHeaderType();
    private GenRequestType reqData=new GenRequestType();
    private LoginRequestType loginData;
    private HttpSession http_session;
    private long lastReload=0;
    private int supLevel;
    private boolean isValid=false;

    private List<DefaultWorkspaceItemType> userWorkspace = new ArrayList<DefaultWorkspaceItemType>();
    private List<GroupListDataItemType> userGroups;
    private List<DNISListDataItemType> userVoiceDNISs;
    private List<DNISListDataItemType> userEmailDNISs;
    private List<GenListDataItemType> userIVRApps;
    private List<GenListDataItemType> userIVRGrps;
    private List<SuperGroupListDataItemType> userSuperGroups;
    private List<GenListDataItemType> userRCodes;
    private Hashtable<String,Vector> userReports_byType=new Hashtable(10);
    private Hashtable<Long, ReportListDataItemType> userReports=new Hashtable(10);
    
    static
    {
        log = LogManager.getLogger("SessionData");
        try {
            switch (NLAService.getConnectedServer()) {
                case NLAService.PRIMARY_SERVER:
                    wsEMIS_port = new EMISwebServiceClient(NLAContext.ACCServerAddress1,NLAContext.ACCServerPort1).getEMISwebPort();
                    session_data_connected_server = NLAService.PRIMARY_SERVER;
                    break;
                case NLAService.SECONDARY_SERVER:
                    wsEMIS_port = new EMISwebServiceClient(NLAContext.ACCServerAddress2,NLAContext.ACCServerPort2).getEMISwebPort();
                    session_data_connected_server = NLAService.SECONDARY_SERVER;
                    break;
                default:
                    log.warn("WARNING, NLAService NOT Connected");
                    break;
            }
        }
        catch (MalformedURLException ex) {
            log.fatal("fatal error, cannot access web services: "+ex.getMessage());
        }
    }


    public SessionData(ServletContext m_ctx, HttpSession session)
    {
        ctx = m_ctx;
        List<SessionData> sessions = nlaCtx.getSessions();
        if(sessions.contains(this))
            log.info("SessionData alresdy exist with session id " + http_session.getId());
        else if(nlaCtx.isSessionExist(session))
            log.info("HttpSession alresdy exist with session id " + http_session.getId());
        else
        {
            log.info("New SessionData is created (session id " + session.getId() + ")");
            http_session = session;
            isValid = true;
            log.info("Num sessions before add " + sessions.size());
            sessions.add(this);
            log.info("Num sessions after add " + sessions.size());

            if (UPDATE_TIMEOUT>0)
            {
                ut = new UpdaterTask();
                timer.schedule(ut,UPDATE_TIMEOUT,UPDATE_TIMEOUT);
            }
        }
    }

    @Override
    protected void finalize() throws Throwable {
        log.info("SessionData DESTROY");
        List<SessionData> sessions=nlaCtx.getSessions();
        sessions.remove(this);
    }

    /**
     * load session's whole  data: work space, user's groups, user reports, etc
     * @param m_loginData - user data
     */
    //public void init(LoginRequestType m_loginData, LoginReturnType loginResp)
    public void init(LoginRequestType m_loginData, LoginReturnType loginResp)
    {
        //log.info("init (session id " + session.getId() + ")");

        this.loginData=m_loginData;
        this.reqHeader.setServiceReqId(1);
        this.supLevel=(int)loginResp.getSupLevel();
        this.reqHeader.setTicket(loginResp.getTicket());
        this.reqData.setRequestInfoHeader(this.reqHeader);
        //this.http_session=session;

        load();
    }

    /**
     * refresh session data
     */
    public void refresh()
    {
        if (System.currentTimeMillis()-lastReload<5000) 
            return;

        List<SessionData> sessions = nlaCtx.getSessions();

        if( !sessions.contains(this))
        {
            log.info("refresh - SessionData NOT Found ");
            return;
        }

        lastReload = System.currentTimeMillis();

        try {
            if ( (wsEMIS_port==null) ){
                log.warn("No connection to WebNLA server, relogin");
                relogin();
            }
            else if(session_data_connected_server != NLAService.getConnectedServer()) {
                log.warn("SessionData:refresh, no connection or connected to wrong server, reconnect");
                reconnect();
            }
        }
        catch(Exception e) {
            log.warn("No connection to WebNLA server");
            wsEMIS_port=null;
        }

        try {
            log.info("REFRESH SessionData " + this.toString() + " (session id " + http_session.getId() + ")");
            loadGroups();
            loadDNISs();
            loadIVRApps();
            loadRCodes();
            loadSuperGroups();
            loadReports();
        }
        catch(Exception e) {
            log.error("SessionData:refresh error: "+e);
        }
    }

    public void reconnect() throws Exception {
        wsEMIS_port=null;
        relogin();
    }

    private void relogin() throws Exception {
        log.info("SessionData::relogin - " + this.toString());
        if (wsEMIS_port==null) {
            wsEMIS_port = NLAService.createConnection(NLAService.getConnectedServer());
            session_data_connected_server = NLAService.getConnectedServer();
        }

        if((wsEMIS_port!=null) && (session_data_connected_server!=NLAService.NOT_CONNECTED))
        {
            log.info("SessionData::relogin - Send userLogin request for session_id " + http_session.getId() + ", UserName " + loginData.getName());
            LoginReturnType loginResp=wsEMIS_port.userLogin(this.loginData);
            if( (loginResp.getLoginEnable()==0) && (loginResp.getResponseInfoHeader().getErrorCause() != 0) )
            {
                log.info("userLogin Fail with ERROR " + loginResp.getResponseInfoHeader().getErrorCause());
                wsEMIS_port=null;
            }
            else
            {
                log.info("userLogin OK for session_id " + http_session.getId() + " with ticket " + loginResp.getTicket());
                this.reqHeader.setTicket(loginResp.getTicket());
            }
        }
        else
        {
            log.info("SessionData::relogin - NOT CONNECTED");
        }
    }

    private void load() throws WebServiceException {
        refresh();
        //loadReports();  //done on refresh
        loadWS();
    }

    public void loadWS()
    {
        log.info("LOAD Workspace");
        // load default workspace
        DefaultWorkspaceRequestType workspaceRequestData=new DefaultWorkspaceRequestType();
        workspaceRequestData.setRequestInfoHeader(reqHeader);
        DefaultWorkspaceReturnType workspaceResponceData=wsEMIS_port.defaultWorkspace(workspaceRequestData);
        this.userWorkspace = workspaceResponceData.getReturnArray();
        log.info("LOAD Workspace - Font Size " + workspaceResponceData.getFontSize() + ", Lang " + workspaceResponceData.getLanguage());
        http_session.setAttribute("dynamicFrontSize", workspaceResponceData.getFontSize());
        //12-Jul-2020 YR BZ#52732
        //http_session.setAttribute("Accept-Language", workspaceResponceData.getLanguage());
        http_session.setAttribute("ws_dynamicFrontSize", workspaceResponceData.getFontSize());
        http_session.setAttribute("ws_Accept-Language", workspaceResponceData.getLanguage());
    }

    public List<DefaultWorkspaceItemType> getWorkspace() {
        return userWorkspace;
    }

    /**
     * retrieve user's groups list
     */
    private void loadGroups()
    { //throws Exception {
        log.debug("loadGroups");
        // get user groups list
        GroupListReturnType resData=wsEMIS_port.groupList(this.reqData);
        userGroups = resData.getReturnArray();
        for (GroupListDataItemType group:userGroups)
        {
            String out1 = null;
            try {
                out1 = new String(group.getGrpName().getBytes("ISO-8859-1"), "UTF-8"); //convertFromUTF8
                group.setGrpName(out1);
            } catch (java.io.UnsupportedEncodingException e) {
                log.error("loadGroups - UnsupportedEncodingException: " + e);
            }
            //log.debug("Group Name: Encoded - " + group.getGrpName() + ", convertFromUTF8 - " + out1 + ", convertToUTF8 - " + out2);
        }
    }

    /**
     * retrieve user's DNIS list
     */
    private void loadDNISs()
    {
        log.debug("loadDNISs");
        // get user DNIS list
        DNISListReturnType resData;
        resData=wsEMIS_port.voiceDNISList(this.reqData);
        userVoiceDNISs = resData.getReturnArray();

        for (DNISListDataItemType dnis:userVoiceDNISs)
        {
            String out1 = null;
            try {
                out1 = new String(dnis.getDnisName().getBytes("ISO-8859-1"), "UTF-8"); //convertFromUTF8
                dnis.setDnisName(out1);
            }
            catch (java.io.UnsupportedEncodingException e) {
                log.error("loadDNISs - UnsupportedEncodingException: " + e);
            }
        }
                
        resData=wsEMIS_port.emailDNISList(this.reqData);
        userEmailDNISs = resData.getReturnArray();

        for (DNISListDataItemType email_dnis:userEmailDNISs)
        {
            String out1 = null;
            try {
                out1 = new String(email_dnis.getDnisName().getBytes("ISO-8859-1"), "UTF-8"); //convertFromUTF8
                email_dnis.setDnisName(out1);
            }
            catch (java.io.UnsupportedEncodingException e) {
                log.error("loadDNISs - UnsupportedEncodingException: " + e);
            }
        }
    }

    private void loadIVRApps()
    {
        log.debug("loadIVRApps");
        // get user IVR list
        GenListReturnType resData;
        resData=wsEMIS_port.ivrAppList(this.reqData);
        userIVRApps = resData.getReturnArray();

        for (GenListDataItemType ivr_app:userIVRApps)
        {
            String out1 = null;
            try {
                out1 = new String(ivr_app.getName().getBytes("ISO-8859-1"), "UTF-8"); //convertFromUTF8
                ivr_app.setName(out1);
            }
            catch (java.io.UnsupportedEncodingException e) {
                log.error("loadIVRApps - UnsupportedEncodingException: " + e);
            }
            //log.debug("Group Name: Encoded - " + group.getGrpName() + ", convertFromUTF8 - " + out1 + ", convertToUTF8 - " + out2);
        }

        resData=wsEMIS_port.ivrPortGroupList(reqData);
        userIVRGrps = resData.getReturnArray();
    }

    private void loadRCodes()
    {
        log.debug("loadRCodes");
        // get user RCode list
        GenListReturnType resData;
        resData=wsEMIS_port.rCodeList(this.reqData);
        userRCodes=resData.getReturnArray();

        for (GenListDataItemType rcode:userRCodes)
        {
            String out1 = null;
            try {
                out1 = new String(rcode.getName().getBytes("ISO-8859-1"), "UTF-8"); //convertFromUTF8
                log.debug("RCode Name: Encoded - " + rcode.getName() + ", convertFromUTF8 - " + out1);
                rcode.setName(out1);
            }
            catch (java.io.UnsupportedEncodingException e) {
                log.error("loadRCodes - UnsupportedEncodingException: " + e);
            }
            //log.debug("RCode Name: Encoded - " + rcode.getName() + ", convertFromUTF8 - " + out1);
        }

    }


    private void loadSuperGroups()
    {
        log.debug("loadSuperGroups");
        // get user SuperGroups list
        SuperGroupListReturnType resData;
        resData=wsEMIS_port.superGroupList(reqData); //SuperGroupList(this.reqData);
        userSuperGroups = resData.getReturnArray();

        for (SuperGroupListDataItemType super_groups:userSuperGroups)
        {
            String out1 = null;
            try {
                out1 = new String(super_groups.getSuperGroupName().getBytes("ISO-8859-1"), "UTF-8"); //convertFromUTF8
                super_groups.setSuperGroupName(out1.replace("<","&lt;").replace(">","&gt;"));
            }
            catch (java.io.UnsupportedEncodingException e) {
                log.error("loadSuperGroups - UnsupportedEncodingException: " + e);
            }
            //log.debug("Group Name: Encoded - " + group.getGrpName() + ", convertFromUTF8 - " + out1 + ", convertToUTF8 - " + out2);
        }
    }


    public List<GenListDataItemType> getIVRApps() {
        return userIVRApps;
    }

    public int getSupLevel() {
        return this.supLevel;
    }

    public boolean isValid() {
        return isValid;
    }

    public void invalidate() {
        isValid = false;
        nlaCtx.getSessions().remove(this);
    }

    /**
     * get list of user's groups
     * @return List<GroupListDataItemType>
     */
    public List<GroupListDataItemType> getGroups() {
        return userGroups;
    }

    /**
     * get list of user's rCodes
     * @return List<GenListDataItemType>
     */
    public List<GenListDataItemType> getRCodes() {
        return userRCodes;
    }

    /**
     * get list of user's DNIS
     * @return List<DNISListDataItemType>
     */
    public List<DNISListDataItemType> getVoiceDNISs() {
        return userVoiceDNISs;
    }

    public List<DNISListDataItemType> getEmailDNISs() {
        return userEmailDNISs;
    }

    public List<DNISListDataItemType> getDNISs() {
        List<DNISListDataItemType> tmp=new ArrayList();
        tmp.addAll(userVoiceDNISs);
        tmp.addAll(userEmailDNISs);
        return tmp;
    }

    /**
     * get list of user's super groups
     * @return List<GroupListDataItemType>
     */
    public List<SuperGroupListDataItemType> getSuperGroups() {
        return userSuperGroups;
    }

    public String getMdIDs(String mdViewLevel, String mdSelected)
    {
        String ids = "";
        switch (mdViewLevel)
        {
            case "Company":
                List<SuperGroupListDataItemType> superGroups=getSuperGroups();
                if(superGroups != null)
                {
                    for (SuperGroupListDataItemType superGroup : superGroups)
                    {
                        ids += superGroup.getSuperGroupId() + ",";
                    }
                }      
                break;
            case "Department":
                int mdSelectedInt = Integer.parseInt(mdSelected);
                List<GroupListDataItemType> groups = null;
                for (SuperGroupListDataItemType superGroup : getSuperGroups())
                {
                    if (superGroup.getSuperGroupId() == mdSelectedInt) {
                        groups = (List<GroupListDataItemType>) superGroup.getReturnArray();
                        break;
                    }
                }
                
                if(groups != null)
                {
                    for (GroupListDataItemType group : groups)
                    {
                        ids += group.getGrpId() + ",";
                    }
                }      
                
                break;
                
            case "Group":
                ids = mdSelected;
                break;
    
        }
        return ids;
    }

    public String getMdRbIDs(String mdViewLevel, String mdRbSelected)
    {
        String ids = "";
        switch (mdViewLevel)
        {
            case "Company":
                break;
            case "Department":
               List<SuperGroupListDataItemType> superGroups=getSuperGroups();
                if(superGroups != null)
                {
                    for (SuperGroupListDataItemType superGroup : superGroups)
                    {
                        ids += superGroup.getSuperGroupId() + ",";
                    }
                }      
                break;
                
            case "Group":
                ids = mdRbSelected;
                break;
    
        }
        return ids;
    }

    public Long getMdSelected(String mdViewLevel, String itemName)
    {
        switch (mdViewLevel)
        {
            case "Company":
                List<SuperGroupListDataItemType> superGroups=getSuperGroups();
                if(superGroups != null)
                {
                    for (SuperGroupListDataItemType superGroup : superGroups)
                    {
                        if (superGroup.getSuperGroupName().equals(itemName)) {
                            return superGroup.getSuperGroupId();
                        }
                    }
                }      
                break;
            case "Department":
                List<GroupListDataItemType> groups = getGroups();
                if(groups != null)
                {
                    for (GroupListDataItemType group : groups)
                    {
                        if (group.getGrpName().equals(itemName)) {
                            return group.getGrpId();
                        }
                    }
                }      
                
                break;   
        }
        return null;
    }
        
    public Long getMdSelectedId(String mdViewLevel, String itemName)
    {
        switch (mdViewLevel)
        {
            case "Company":
                return (long)64;
            case "Department":
                List<SuperGroupListDataItemType> superGroups=getSuperGroups();
                if(superGroups != null)
                {
                    for (SuperGroupListDataItemType superGroup : superGroups)
                    {
                        if (superGroup.getSuperGroupName().equals(itemName)) {
                            return superGroup.getSuperGroupId();
                        }
                    }
                }      
                break;   
        }
        return null;
    }
        
    public String getMdSelectedName(String mdViewLevel, String itemId)
    {
        int mdSelectedInt;
        List<SuperGroupListDataItemType> superGroups=getSuperGroups();
        List<GroupListDataItemType> groups = getGroups();
        switch (mdViewLevel)
        {
            case "Company":
                if(superGroups != null)
                {
                    for (SuperGroupListDataItemType superGroup : superGroups)
                    {
                        if (superGroup.getSuperGroupId() == 64) {
                            return superGroup.getSuperGroupName();
                        }
                    }
                }
                break;   
            case "Department":
                mdSelectedInt = Integer.parseInt(itemId);
                if(superGroups != null)
                {
                    for (SuperGroupListDataItemType superGroup : superGroups)
                    {
                        if (superGroup.getSuperGroupId() == mdSelectedInt) {
                            return superGroup.getSuperGroupName();
                        }
                    }
                }
                break;   
            case "Group":
                mdSelectedInt = Integer.parseInt(itemId);
                if(groups != null)
                {
                    for (GroupListDataItemType group : groups)
                    {
                        if (group.getGrpId() == mdSelectedInt) {
                            return group.getGrpName();
                        }
                    }
                }      
                break;   
        }
        return "";
    }
        
    public LoginRequestType getLoginData() {
        return loginData;
    }

    public HttpSession getHttpSession() {
        return http_session;
    }

    public List<GenListDataItemType> getIVRGroups() {
        return userIVRGrps;
    }

    /**
     * load user's reports
     */
    public void loadReports()
    {
        log.info("LOAD Reports");
        // load user reports
        ReportListDataRequestType reportsReqData = new ReportListDataRequestType();
        reportsReqData.setRequestInfoHeader(reqHeader);
        ReportListDataReturnType retData = wsEMIS_port.reportListData(reportsReqData);		//get user saved reports
        // bind user reports to renders
        List<ReportListDataItemType> reportList = (List<ReportListDataItemType>) retData.getReturnArray();
        ReportListDataItemType report;
        String renderName;
        String renderUrl;
        Properties renders = (Properties)ctx.getAttribute("nlatRenders");
        Vector<ReportListDataItemType> rendersList;
        String key;
        userReports.clear();
        userReports_byType.clear();
        for (ReportListDataItemType item : reportList)
        {
            report = item;
            renderName = report.getRenderName();
            renderUrl = renders.getProperty(renderName,"");
            if (!"".equals(renderUrl))
            {
                try
                {
                    report.setParams(renderUrl+"?"+report.getParams());
                    log.debug("Load Report (decoded) " + URLDecoder.decode(report.getReportName(),"UTF-8"));
                    report.setReportName(URLDecoder.decode(report.getReportName(),"UTF-8"));
                    userReports.put(report.getReportId(), report);

                    key = renderUrl.substring(0, renderUrl.indexOf("."));
                    rendersList = userReports_byType.get(key);
                    if (rendersList == null)
                        rendersList = new Vector<>();
                    rendersList.add(report);
                    userReports_byType.put(key, rendersList);
                }
                catch(Exception e) {
                    log.error("Exception loading report " + report.getReportName() + ":" + e.getMessage());
                }
            }
        }
    }

    /**
     * get user's reports
     * @return Hashtable<Long,ReportListDataReturnItemType>
     */
    public Hashtable<Long, ReportListDataItemType> getReports() {
        return userReports;
    }

    /**
     * get user reports by category
     * @return Hashtable<String,Vector>
     */
    public Hashtable<String,Vector> getReportsByType() {
        return userReports_byType;
    }

    public String getTicket() {
        return reqHeader.getTicket();
    }

    public int getMaxAgentsInBriefReport() {
        return MaxAgentsInBriefReport;
    }

    public void logout(boolean user_logout)
    {
        // send logout command
        if(user_logout)
        {
            log.info("logout");
            try {
                wsEMIS_port.userLogout(reqData);
            }
            catch(Exception e) {
                log.warn("no coonection, unable to logout");
            }
        }

        ut.cancel();
        timer.purge();

        List<SessionData> sessions=nlaCtx.getSessions();
        log.info("Num sessions before remove " + sessions.size());
        sessions.remove(this);
        log.info("Num sessions after remove " + sessions.size());
    }

    
    /**
    * Session data updater
    */
    private class UpdaterTask extends TimerTask {
        @Override
        public void run () {
            refresh();
        }
    }

}
