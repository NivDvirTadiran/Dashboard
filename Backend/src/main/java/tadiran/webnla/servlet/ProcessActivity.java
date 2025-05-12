package tadiran.webnla.servlet;

import jakarta.servlet.annotation.WebServlet;
import tadiran.emisweb.*;
import tadiran.webnla.NLAContext;
import tadiran.webnla.bean.SessionData;
import tadiran.webnla.endpoint.EMISwebServiceClient;
import tadiran.webnla.tag.LocaleMessage;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.MalformedURLException;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.util.Collections;
import java.util.List;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;

/**
 * handler to process/send webNLA commands from GUI to NLA(Epic)
 * @author evgeniv
 */
@WebServlet(urlPatterns = "/processActivity",name = "ProcessActivity")
public class ProcessActivity extends HttpServlet
{
    
    private static final String SAVE_WS="SAVE_WS";
    private static final String CHANGE_FONT_SIZE="CHANGE_FONT_SIZE";
    private static final String CHANGE_LANG="CHANGE_LANG";
    private static final String BACK_WS_LANG="BACK_WS_LANG";
    private static final String SHOW_LOGGED_IN_STATE="SHOW_LOGGED_IN_STATE";
    private static final String SAVE_REPORT="SAVE_REPORT";
    private static final String DELETE_REPORT="DELETE_REPORT";
    private static final String CHAT="START_CHAT";
    private static final String LOGIN="LOGIN";
    private static final String LOGOUT="LOGOUT";
    private static final String LOGOUT_CODE="LOGOUT_CODE";
    private static final String RELEASE="RELEASE";
    private static final String RESUME="RESUME";
    private static final String RECORD="START_RECORD";
    private static final String BREAKIN="BREAK_IN";
    private static final String WHISPER="WHISPER";
    private static final String MONITOR="SILENT_MONITOR";
    private static final int SAVE_ERROR_PUBLIC_AS_PRIVATE=303;
    private static final int ACTIVITY_ERROR_SUP_NOT_AGENT=503;

    private static EMISwebPort wsEMIS_port;
    private static int process_activity_connected_server = NLAService.NOT_CONNECTED;
    private static final Logger log;
    
    static {
        log = LogManager.getLogger("ProcessActivity");
        try {
            switch (NLAService.getConnectedServer()) {
                case NLAService.PRIMARY_SERVER:
                    wsEMIS_port = new EMISwebServiceClient(NLAContext.ACCServerAddress1,NLAContext.ACCServerPort1).getEMISwebPort();
                    process_activity_connected_server = NLAService.PRIMARY_SERVER;
                    break;
                case NLAService.SECONDARY_SERVER:
                    wsEMIS_port = new EMISwebServiceClient(NLAContext.ACCServerAddress2,NLAContext.ACCServerPort2).getEMISwebPort();
                    process_activity_connected_server = NLAService.SECONDARY_SERVER;
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

    @Override
    protected void doPost (HttpServletRequest req, HttpServletResponse resp) throws IOException
    {
        String activity=req.getParameter("activity");
        
        if (activity != null)
        {
            if (activity.equalsIgnoreCase(CHANGE_FONT_SIZE))
            {
                String font_size_param[]=req.getParameterValues("font_size_param");
                req.getSession().setAttribute("dynamicFrontSize", font_size_param[0]);
                log.info("CHANGE_FONT_SIZE setAttribute - dynamicFrontSize: " + font_size_param[0]);
                return;
            }
            else if (activity.equalsIgnoreCase(CHANGE_LANG))
            {
                String lang_param[]=req.getParameterValues("lang_param");
                req.getSession().setAttribute("Accept-Language", lang_param[0]);
                log.info("CHANGE_LANG setAttribute - Accept-Language: " + lang_param[0]);
                return;
            }
            else if (activity.equalsIgnoreCase(BACK_WS_LANG)) 
            {
                String wsLang = (String)req.getSession().getAttribute("ws_Accept-Language");
                req.getSession().setAttribute("Accept-Language", wsLang);
                log.info("BACK_WS_LANG setAttribute - Accept-Language: " + wsLang);
                String wsFont = (String)req.getSession().getAttribute("ws_dynamicFrontSize");
                req.getSession().setAttribute("dynamicFrontSize", wsFont);
                log.info("BACK_WS_LANG setAttribute - dynamicFrontSize:" + wsFont);
                return;
            }
            else if (activity.equalsIgnoreCase(SHOW_LOGGED_IN_STATE))
            {
                String show_logged_in_param[]=req.getParameterValues("show_logged_in_only");
                String unique_nlat_name[]=req.getParameterValues("unique_nlat_name");

                //req.getSession().setAttribute("ShowLoggedInOnly", show_logged_in_param[0]);
                req.getSession().setAttribute("ShowLoggedInOnly:"+unique_nlat_name[0], show_logged_in_param[0]);
                log.info("LOGGED_IN_STATE setAttribute: " + unique_nlat_name[0] + "-" + show_logged_in_param[0]);
                log.info("unique_nlat_name: " + unique_nlat_name[0]);

                return;
            }
        }
        
        //req.setCharacterEncoding("UTF-8");
        resp.setContentType("text/html");
        //10-Jun-2019 YR BZ#49947
        resp.setCharacterEncoding("UTF-8");
        PrintWriter out = resp.getWriter();
        String respText="";

        
        if (activity != null)
        {
            String langName=(String)req.getSession().getAttribute("Accept-Language");
            String fontSize=(String)req.getSession().getAttribute("dynamicFrontSize");
            String respParams="";
            
            HttpSession session = req.getSession();
            SessionData sessionData=(SessionData)session.getAttribute("sessionData");
            ResponseInfoHeaderType respData=null;

            RequestInfoHeaderType reqHeader=new RequestInfoHeaderType();
            reqHeader.setServiceReqId(1);
            reqHeader.setTicket(sessionData.getTicket());

            RequestSaveReportType reqRepData=new RequestSaveReportType();
            ResponseSaveReportType resRepData;
            reqRepData.setRequestInfoHeader(reqHeader);
            ReportListDataItemType rep_item;
            long report_id;
            
            log.debug("Process " + activity.toUpperCase() + " request");
            try {
                if( process_activity_connected_server != NLAService.getConnectedServer() )
                {
                    log.info("*no connection or connected to wrong server, reconnect..");
                    log.info("*call NLAService.createConnection(NLAService.getConnectedServer()");
                    wsEMIS_port = NLAService.createConnection(NLAService.getConnectedServer());
                    process_activity_connected_server = NLAService.getConnectedServer();
                }
                
                if (activity.equalsIgnoreCase(SAVE_WS))
                {
                    String rep_dim[]=req.getParameterValues("rep_dim");
                    String rep_loc[]=req.getParameterValues("rep_loc");
                    String rep_name[]=req.getParameterValues("rep_name");
                    String rep_id[]=req.getParameterValues("rep_id");
                    String rep_params[]=req.getParameterValues("rep_params");

                    SaveWorkspaseRequestType reqWSData=new SaveWorkspaseRequestType();
                    List<DefaultWorkspaceItemType> wsData= (List<DefaultWorkspaceItemType>) reqWSData.getRequestArray();
                    DefaultWorkspaceItemType ws_item;

                    if (rep_name != null)
                    {
                        String tmp[];
                        for(int i=0; i<rep_name.length; i++)
                        {
                            ws_item=new DefaultWorkspaceItemType();
                            tmp=rep_loc[i].split("x");
                            ws_item.setX(Short.parseShort(tmp[0]));
                            ws_item.setY(Short.parseShort(tmp[1]));
                            tmp=rep_dim[i].split("x");
                            ws_item.setWidth(Short.parseShort(tmp[0]));
                            ws_item.setHeight(Short.parseShort(tmp[1]));

                            if (rep_name[i].startsWith("*"))
                            {
                                //unsaved report
                                rep_name[i]=rep_name[i].substring(2);
                                report_id=0;
                                try {
                                    report_id=Long.parseLong(rep_id[i]);
                                    // updated report
                                    rep_item=sessionData.getReports().get(report_id);
                                    if (rep_item.getIsPublic()==1)
                                        report_id=0;
                                }
                                catch(Exception e) {
                                    // new report
                                    rep_item=new ReportListDataItemType();
                                    rep_item.setRenderName(rep_id[i]);
                                    rep_item.setReportName(rep_name[i]);
                                }
                                rep_item.setParams(rep_params[i]);
                                rep_item.setIsPublic(0);
                                rep_item.setReportId(report_id);

                                reqRepData.setRequestItem(rep_item);
                                resRepData=wsEMIS_port.saveReport(reqRepData);
                                if (resRepData.getErrorCause()!=0)
                                    throw new Exception("ERR"+resRepData.getErrorCause()+", "+LocaleMessage.getMessage("activity.InvSave", langName)+" '"+rep_item.getReportName()+"' "+LocaleMessage.getMessage("activity.Report", langName));

                                if (report_id==0)
                                    report_id=resRepData.getReportId();
                            }
                            else
                                report_id=Long.parseLong(rep_id[i]);

                            ws_item.setReportId(report_id);
                            wsData.add(ws_item);
                            respParams+=","+report_id;	//saved report id
                        }
                    }

                    log.debug("Process " + activity.toUpperCase() + " request - fontSize " + fontSize + ", Lang " + langName);
                    reqWSData.setRequestInfoHeader(reqHeader);
                    if(fontSize==null)
                        fontSize="Small";
                    reqWSData.setFontSize(fontSize);
                    reqWSData.setLanguage(langName);
                    respData=wsEMIS_port.saveWorkspase(reqWSData);
                    sessionData.loadWS();
                    respText=LocaleMessage.getMessage("activity.SuccWSSave", langName);
                }
                else if (activity.equalsIgnoreCase(SAVE_REPORT)) 
                {
                    String rep_name=req.getParameter("rep_name");
                    String rep_public=req.getParameter("rep_public");
                    String rep_id=req.getParameter("rep_id");
                    String rep_params=req.getParameter("rep_params");

                    rep_item=new ReportListDataItemType();
                    rep_item.setIsPublic((rep_public!=null)?1:0);
                    rep_item.setParams(rep_params);
                    //rep_item.setReportName(rep_name);
                    //String rep_name_str=new String(((String)rep_name).getBytes("ISO-8859-1"), "UTF-8");
                    //rep_item.setReportName(rep_name_str);
                    //10-Jun-2019 YR BZ#49999
                    String rep_name_encode=URLEncoder.encode(rep_name,"UTF-8");
                    rep_item.setReportName(rep_name_encode);
                    //String rep_name_decode=URLDecoder.decode(rep_name,"UTF-8");
                    //rep_item.setReportName(rep_name_decode);
                    report_id=0;
                    log.info("Activity " + activity.toUpperCase() + ": Report Name - " + rep_name + "(encoded " + rep_name_encode + "), Id - " + rep_id + ", Params - " + rep_params);
                    try {
                        report_id=Long.parseLong(rep_id);
                        // updated report
                        ReportListDataItemType rep_item2=sessionData.getReports().get(report_id);
                        if(rep_item2 == null)
                            rep_item.setRenderName(rep_name);
                        else
                        {
                            rep_item.setRenderName(rep_item2.getRenderName());
                            if (!rep_item2.getReportName().equals(rep_name))
                                report_id=0;
                        }


                        rep_item.setRenderName(rep_item2.getRenderName());
                        if (!rep_item2.getReportName().equals(rep_name))
                            report_id=0;
                    }
                    catch(NumberFormatException e) {
                        // new report
                        rep_item.setRenderName(rep_id);
                    }
                    catch(Exception e2) {
                    }
                    rep_item.setReportId(report_id);

                    reqRepData.setRequestItem(rep_item);
                    resRepData=wsEMIS_port.saveReport(reqRepData);

                    respData=new ResponseInfoHeaderType();
                    respData.setErrorCause(resRepData.getErrorCause());
                    if (resRepData.getErrorCause()!=0) //save report fail
                        throw new Exception("");

                    //success
                    if (report_id==0)
                        report_id=resRepData.getReportId();
                    respParams=report_id+",'"+rep_name+"'";
                    respText=LocaleMessage.getMessage("activity.SuccSave", langName);
                }
            }
            catch(Exception e) {
                log.error("ERROR: Can not process "+activity.toUpperCase()+" request:"+e.toString());
                if (e.getMessage()!=null)
                    respText=e.getMessage();
            }

            //proccess resp. params
            if (respParams.startsWith(","))
                respParams=respParams.substring(1);

            if (respParams.equals(""))
                respParams="null";
            else
                respParams="["+respParams+"]";

            // write resp. code
            out.println("<script type='text/javascript'>");
            //2-Oct-2019 YR BZ#50926
            if (respData == null)
            { //fail
                respText = LocaleMessage.getMessage("activity.SaveReport", langName) + " " + LocaleMessage.getMessage("activity.Error", langName) + ":\\n" + LocaleMessage.getMessage("activity.ActivityError", langName);
            }
            else if (respData.getErrorCause() != 0)
            {
                respText = LocaleMessage.getMessage("activity.SaveReport", langName) + " " + LocaleMessage.getMessage("activity.Error", langName) + ":\\n";
                if(respData.getErrorCause()==SAVE_ERROR_PUBLIC_AS_PRIVATE)
                {
                    respText = respText + LocaleMessage.getMessage("activity.InvSavePublicAsPrivate", langName);
                    //respText="ERR: "+respData.getErrorCause();
                }
                else
                {
                    respText = respText + "ERR: " + respData.getErrorCause();
                }
            }
            else //success
                sessionData.loadReports();

            out.println("parent.closeSubmitWin("+respParams+",'"+respText+"');");
            out.println("</script>");
        }
        else {
            String sleep=req.getParameter("sleep");
            if (sleep!=null) {
                try {
                    if (sleep.isEmpty())
                        sleep="1000";
                    
                    Long period=Long.parseLong(sleep);
                    Thread.sleep(period);
                    respText="sleep: "+period+"ms";
                }
                catch (Exception ex) {
                    respText="cannot sleep: "+ex;
                }
            }
        }

        out.println(respText);
        out.close();
    }

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws IOException {
        String activity = req.getParameter("activity");
        if (activity == null) {
            resp.setStatus(HttpServletResponse.SC_BAD_REQUEST);
            resp.getWriter().write("Activity parameter is required");
            return;
        }

        activity = activity.toUpperCase();
        String langName = (String)req.getSession().getAttribute("Accept-Language");
        resp.setCharacterEncoding("UTF-8");
        HttpSession session = req.getSession();
        SessionData sessionData=(SessionData)session.getAttribute("sessionData");
        String agentId=req.getParameter("agentId");
        String code=req.getParameter("code");

        ResponseInfoHeaderType respData=null;
        RequestInfoHeaderType reqHeader=new RequestInfoHeaderType();
        reqHeader.setServiceReqId(1);
        reqHeader.setTicket(sessionData.getTicket());

        log.debug("Process " + activity + " request");
        try {
            //1-Jun-2021 YR BZ#54488
            if( process_activity_connected_server != NLAService.getConnectedServer() )
            {
                log.info("*no connection or connected to wrong server, reconnect..");
                log.info("*call NLAService.createConnection(NLAService.getConnectedServer()");
                wsEMIS_port = NLAService.createConnection(NLAService.getConnectedServer());
                process_activity_connected_server = NLAService.getConnectedServer();
            }
                
            // ACD features
            if (activity.startsWith("LOG") && !activity.equals("LOGOUT_CODE"))  //login, logout
            {
                String groupId=req.getParameter("groupId");
                ACDMultiRequestType reqData=new ACDMultiRequestType();
                reqData.setRequestInfoHeader(reqHeader);
                String ids[] = null;
                // set id param, prepare mulit ids param
                if (agentId.indexOf(",")==-1) { // login/logout by agent
                    reqData.setId(Integer.parseInt(agentId));
                    if((groupId != null) && !groupId.isEmpty())
                        ids=groupId.split(",");
                }
                else {
                    reqData.setId(Integer.parseInt(groupId));
                    ids=agentId.split(",");
                }

                // construct multi id param
                if(ids != null)
                {
                    List<Long> reqData_ids= reqData.getMultiId();
                    for (int i=0; i<ids.length; i++)
                        reqData_ids.add(Long.parseLong(ids[i]));
                    }

                if (activity.equals("LOGIN"))
                    reqData.setOperation(LOGIN);
                else if (activity.equals("LOGOUT"))
                    reqData.setOperation(LOGOUT);

                // call operation
                if (agentId.indexOf(",")==-1)
                    respData=wsEMIS_port.loginManagerOneAgentToMultiGroups(reqData);
                else
                    respData=wsEMIS_port.loginManagerMultiAgentsToOneGroup(reqData);
            }
            else if (activity.equals("RELEASE") || activity.equals("RESUME") || activity.equals("LOGOUT_CODE"))
            {
                ACDRequestType reqData=new ACDRequestType();
                reqData.setRequestInfoHeader(reqHeader);
                reqData.setAgentId(Integer.parseInt(agentId));
                reqData.setGroupId(0);

                if (activity.equals("RELEASE"))
                {
                    reqData.setOperation(RELEASE);
                    respData=wsEMIS_port.commandAgentReleaseResume(reqData);
                }
                else if (activity.equals("RESUME")) 
                {
                    reqData.setOperation(RESUME);
                    respData=wsEMIS_port.commandAgentReleaseResume(reqData);
                }
                else if (activity.equals("LOGOUT_CODE"))
                {
                    String code_decode= URLDecoder.decode(code,"UTF-8");
                    String code_encode=URLEncoder.encode(code,"UTF-8");
                    reqData.setOperation(LOGOUT_CODE);
                    log.info("AgentId:" + agentId + "  LOGOUT_CODE:  " + code + " <decoded:" + code_decode + ">,<encoded:" + code_encode + ">");
                    reqData.setCode(code_encode);
                    respData=wsEMIS_port.commandAgentReleaseResume(reqData);
                }
            }
            else if (activity.equals(DELETE_REPORT)) 
            {
                RequestDeleteReportType reqData=new RequestDeleteReportType();
                reqData.setRequestInfoHeader(reqHeader);
                reqData.setReportId(Integer.parseInt(req.getParameter("reportId")));
                respData=wsEMIS_port.deleteReport(reqData);
                if (respData.getErrorCause()==0)
                    sessionData.loadReports();
            }
            else 
            {
                // Telephone features
                TFRequestType reqTFData=new TFRequestType();
                reqTFData.setRequestInfoHeader(reqHeader);
                reqTFData.setAgentId(Integer.parseInt(agentId));

                if (activity.equals("CHAT"))
                {
                    reqTFData.setOperation(CHAT);
                    ChatResponseType chatResp=wsEMIS_port.commandAgentStartChat(reqTFData);

                    if (chatResp.getErrorCause()==0)
                    {
                        String adminName=(String)session.getAttribute("userName");
                        resp.sendRedirect("http://tomcat.cs.com/epicChat/EpicChat?epic_start_page=EPChat_StartAgent.jsp&Cust_name="+adminName+"&session_id="+chatResp.getSessionId()+"&action=admin2agt");
                        return;
                    }
                }
                else if (activity.equals("RECORD"))
                {
                    reqTFData.setOperation(RECORD);
                    respData=wsEMIS_port.commandAgentStartRecord(reqTFData);
                }
                else if (activity.equals("BREAKIN")) 
                {
                    reqTFData.setOperation(BREAKIN);
                    respData=wsEMIS_port.commandAgentTelephonyFeatures(reqTFData);
                }
                else if (activity.equals("WHISPER"))
                {
                    reqTFData.setOperation(WHISPER);
                    respData=wsEMIS_port.commandAgentTelephonyFeatures(reqTFData);
                }
                if (activity.equals("MONITOR"))
                {
                    reqTFData.setOperation(MONITOR);
                    respData=wsEMIS_port.commandAgentTelephonyFeatures(reqTFData);
                }
            }
        }
        catch(Exception e) {
            log.error("ERROR: Can not process "+activity.toUpperCase()+" request:"+e.getMessage());
        }
		
        resp.setContentType("text/html");
        PrintWriter out = resp.getWriter();
        out.println("<script type='text/javascript'>");
        String errorMessage = "";
        if( (respData == null) || (respData.getErrorCause() != 0) )
        {
            errorMessage = "'" + LocaleMessage.getMessage("activity.ActivityError", langName) + " " + activity.toUpperCase() + " " + LocaleMessage.getMessage("activity.Activity", langName) + ":\\n";
            if(respData == null)
            {
                errorMessage += "Got NULL Response";
            }
            else if(respData.getErrorCause() == ACTIVITY_ERROR_SUP_NOT_AGENT)
            {
                errorMessage += LocaleMessage.getMessage("activity.ActivitySupNotAgent", langName);
            }
            errorMessage += "'";
        }
        out.println("parent.closeActivityWin("+errorMessage+");");
        out.println("</script>");
        out.close();
    }

    private static List<GenListDataItemType>[] getLoggedObject(String objRef, String ticket)
    {
        List<GenListDataItemType>[] result=new List[2];

        RequestInfoHeaderType reqHeader=new RequestInfoHeaderType();
        reqHeader.setServiceReqId(1);
        reqHeader.setTicket(ticket);
        LoginManagerRequestType reqData=new LoginManagerRequestType();
        String[] obj=objRef.split(":");
        reqData.setId(Integer.parseInt(obj[1].split("\\.")[0]));
        reqData.setRequestInfoHeader(reqHeader);

        try
        {
            if( process_activity_connected_server != NLAService.getConnectedServer() )
            {
                log.info("*no connection or connected to wrong server, reconnect..");
                log.info("*call NLAService.createConnection(NLAService.getConnectedServer()");
                wsEMIS_port = NLAService.createConnection(NLAService.getConnectedServer());
                process_activity_connected_server = NLAService.getConnectedServer();
            }

            if (obj[0].equalsIgnoreCase("group"))
            {
                result[0]= (List<GenListDataItemType>) wsEMIS_port.loginManagerAgentsPerGroup(reqData).getReturnArray();
                result[1]= (List<GenListDataItemType>) wsEMIS_port.loginManagerAvailableAgentsForGroup(reqData).getReturnArray();

            }
            else
            {
                result[0]= (List<GenListDataItemType>) wsEMIS_port.loginManagerGroupsPerAgent(reqData).getReturnArray();
                result[1]= (List<GenListDataItemType>) wsEMIS_port.loginManagerAvailableGroupsForAgent(reqData).getReturnArray();
            }
        }
        catch(Exception e) {
            log.error("ERROR: cannot process LoginManagerXXXper"+obj[0]+" request. "+e.getMessage());
            result=null;
        }

        return result;
    }

    public static List<GenListDataItemType>[] getLoggedGroups(String id, String ticket) {
        return getLoggedObject("agent:"+id,ticket);
    }

    public static List<GenListDataItemType>[] getLoggedAgents(String id, String ticket) {
        return getLoggedObject("group:"+id,ticket);
    }
}
