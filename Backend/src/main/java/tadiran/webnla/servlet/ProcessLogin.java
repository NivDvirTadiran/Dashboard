package tadiran.webnla.servlet;

import jakarta.servlet.annotation.WebServlet;
import tadiran.emisweb.EMISwebPort;
import tadiran.emisweb.LoginRequestType;
import tadiran.emisweb.LoginReturnType;
import tadiran.webnla.NLAContext;
import tadiran.webnla.bean.SessionData;
import tadiran.webnla.endpoint.EMISwebServiceClient;
import tadiran.webnla.tag.LocaleMessage;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URLEncoder;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.*;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;
import static tadiran.webnla.filter.AuthTokenFilter.getUserNameFromJwtToken;
//import org.apache.tomcat.util.buf.StringUtils;

/**
 * user login handler
 * @author evgeniv
 */
//@WebServlet(urlPatterns = {"/login"})
public class ProcessLogin extends HttpServlet 
{
    private static final Logger log = LogManager.getLogger("ProcessLogin");
    private static String appId;


    @Override
    protected void doPost (HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException
    {
        if (appId==null)
            appId=getServletContext().getInitParameter("appID");

        EMISwebPort wsEMIS_port = null;
        String langName = req.getParameter("lang");
        req.setCharacterEncoding("UTF-8");
        

        try {
            HttpSession session = req.getSession(true);
            //String langName=req.getParameter("lang");
            resp.setCharacterEncoding("UTF-8");
            resp.setContentType("text/html");

            String user=req.getParameter("user");
            String pass=req.getParameter("pass");

            String ipAddress = getClientIpAddress(req);
                
            log.info("login request: USER=" + user+ " (encoded " + URLEncoder.encode(user,"UTF-8") + ") PASS=" + pass + ", Language=" + langName + ", session=" + session.getId() + ", IP Addr=" + ipAddress);
            //String ip = req.getRemoteAddr();
            //String headerClientIp = req.getHeader("Client-IP");
            //String headerXForwardedFor = req.getHeader("X-Forwarded-For");
                
            //if (ip.isEmpty() && !headerClientIp.isEmpty()) {
            //    ip = headerClientIp;
            //}
            //else if (!headerXForwardedFor.isEmpty()) {
            //    ip = headerXForwardedFor;
            //}
                
            //log.info("login request: USER="+user+" (encoded "+URLEncoder.encode(user,"UTF-8")+") PASS="+pass);
            String sso_enable=(String)session.getAttribute("SSO_ENABLE");
            if(sso_enable!=null)
            {
                pass="SSO";
            }
            
            SessionData sessionData=(SessionData)session.getAttribute("sessionData");
            if (sessionData==null)
            {
                //session.setAttribute("userName", URLEncoder.encode(user,"UTF-8"));
                if (user.startsWith("GateUser: ") && getUserNameFromJwtToken(pass).equals(user.substring(10)))
                {
                    log.info("gate password validated");
                    user = user.substring(10, user.length());
                    pass = "userGate";
                }
                else if (pass.equals("userGate"))
                {
                    pass = "PasswordForbidden";
                }
                        
                session.setAttribute("userName", user);
                session.setAttribute("Accept-Language", langName);
                session.setAttribute("pass", pass);
                LoginRequestType loginData=new LoginRequestType();
                loginData.setServiceReqId(1);
                loginData.setApplicationId(appId);
                //loginData.setName(user);
                loginData.setName(URLEncoder.encode(user,"UTF-8"));
                //loginData.setName(URLDecoder.decode(user,"UTF-8"));
                loginData.setPass(pass);
                loginData.setSessionId(session.getId());
                loginData.setIpAddress(ipAddress);

                
                //EMISwebPort wsEMIS_port=null;
                LoginReturnType loginResp=null;
                
                if(wsEMIS_port == null)
                {
                    wsEMIS_port = establishServerConnection();
                }
            
                //NLAContext.getInstance().invalidateOldUserSession(loginData);

                if(wsEMIS_port != null)
                {
                    log.info("Send userLogin request for session_id " + session.getId());
                    loginResp = wsEMIS_port.userLogin(loginData);
                    if (loginResp.getLoginEnable()!=0 && req.isRequestedSessionIdValid())
                    {
                        log.info("LOGIN Success (session_id " + session.getId() + ")");
                        //18-Mar-2019 YR BZ#49276
                        Cookie userCookie = new Cookie("userName", URLEncoder.encode(user,"UTF-8"));
                        //Cookie userCookie = new Cookie("userName", user);
                        resp.addCookie(userCookie);


                        log.info("Create new SessionData");
                        sessionData = new SessionData(getServletContext(),session);
                        if(!sessionData.isValid())
                        {
                            log.warn("sessionData Created is NOT VALID");
                        }
                        sessionData.init(loginData,loginResp);

                        session.setAttribute("sessionData",sessionData);
                        session.setAttribute("userLogin","TRUE");
                        session.removeAttribute("loginError");
                        resp.sendRedirect("index.jsp");
                        return;
                    }
                    //17-Jul-2023 YR BZ#58100
                    else if(!req.isRequestedSessionIdValid())
                    {
                        log.info("LOGIN Success BUT session_id " + session.getId() + " is NOT VALID");
                    }
                    else
                    {
                        int errorCode=(int)loginResp.getResponseInfoHeader().getErrorCause();
                        log.info("login fail: "+errorCode);
                        switch(errorCode)
                        {
                            case 200:
                            case 203:   req.setAttribute("loginError",LocaleMessage.getMessage("login.InvCred", langName));
                            break;
                            case 201:
                            case 202:
                            case 204:   req.setAttribute("loginError",LocaleMessage.getMessage("login.MaxUsers", langName));
                            break;
                            case 207:   req.setAttribute("loginError",LocaleMessage.getMessage("login.RemLogged", langName));
                            break;
                            case 700:   req.setAttribute("loginError",LocaleMessage.getMessage("login.InvUser", langName));
                            break;
                            default:    req.setAttribute("loginError",LocaleMessage.getMessage("login.Error", langName)+": "+errorCode);
                            break;
                        }
                    }
                }
            }
            else
            {
                log.info("SessionData already exist");
            }
        }
        catch(Exception e) {
            log.error("Cannot process WebNLA service: "+e.getMessage());
            req.setAttribute("loginError",LocaleMessage.getMessage("login.InvSrv", langName));
        }

        log.info(" Forward to LOGIN Page");
        req.getRequestDispatcher("login.jsp").forward(req, resp);
        wsEMIS_port=null;
    }

    public static EMISwebPort establishServerConnection() throws MalformedURLException {
        EMISwebPort wsEMIS_port = null;
        log.info("NLAService.getConnectedServer(): " + NLAService.getConnectedServer());

        switch (NLAService.getConnectedServer())
        {
            case NLAService.PRIMARY_SERVER:
                log.info("wsEMIS_port is null, NLAService connected to PRIMARY_SERVER");
                wsEMIS_port = new EMISwebServiceClient(NLAContext.ACCServerAddress1,NLAContext.ACCServerPort1).getEMISwebPort();
                break;
            case NLAService.SECONDARY_SERVER:
                log.info("wsEMIS_port is null, NLAService connected to SECONDARY_SERVER");
                wsEMIS_port = new EMISwebServiceClient(NLAContext.ACCServerAddress2,NLAContext.ACCServerPort2).getEMISwebPort();
                break;
            default:
                log.info("wsEMIS_port is null, NLAService NOT connected");
                break;
        }

        return wsEMIS_port;
    }

    public static String getClientIpAddress(HttpServletRequest req) {
        for (String header : HEADERS_TO_TRY) {
            String ip = req.getHeader(header);

            if (ip != null && !ip.isEmpty() && !"unknown".equalsIgnoreCase(ip)) {
                if ("0:0:0:0:0:0:0:1".equals(ip))
                    return "127.0.0.1";
                else
                    return ip;
            }
        }

        return req.getRemoteAddr();
    }

    private static final String[] HEADERS_TO_TRY = {
            "X-Forwarded-For",
            "Proxy-Client-IP",
            "WL-Proxy-Client-IP",
            "HTTP_X_FORWARDED_FOR",
            "HTTP_X_FORWARDED",
            "HTTP_X_CLUSTER_CLIENT_IP",
            "HTTP_CLIENT_IP",
            "HTTP_FORWARDED_FOR",
            "HTTP_FORWARDED",
            "HTTP_VIA",
            "REMOTE_ADDR" };
}
