package tadiran.webnla.servlet;

import jakarta.servlet.annotation.WebListener;
import tadiran.webnla.bean.SessionData;
import jakarta.servlet.http.HttpSession;
import jakarta.servlet.http.HttpSessionEvent;
import jakarta.servlet.http.HttpSessionListener;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;


/**
 * SessionsListener
 *
 * @author evgeniv
 */
@WebListener
public class SessionsListener implements HttpSessionListener
{
    private static final Logger logger = LogManager.getLogger(SessionsListener.class);

    @Override
    public void sessionCreated (HttpSessionEvent se)
    {
        logger.info("sessionCreated "  + se.getSession().getId());
        HttpSession session=se.getSession();
        session.setMaxInactiveInterval(45);
        session.setAttribute("Accept-Language", "en");
        session.setAttribute("DP_APP_CENTER",(NLAService.DP_APP_CENTER ? "DP_ENABLE" : "DP_DISABLE"));
    }

    /* Session Invalidation Event */
    @Override
    public void sessionDestroyed (HttpSessionEvent se)
    {
        logger.info("sessionDestroyed " + se.getSession().getId());
        HttpSession session=se.getSession();
        SessionData sessData=(SessionData)session.getAttribute("sessionData");
        if (sessData!=null)
        {
            logger.info("LOGOUT session expired, userName=" + session.getAttribute("userName"));
            sessData.logout(true);
            sessData.invalidate();
            session.removeAttribute("sessionData");
            sessData=null;
            session.invalidate();
        }
    }
}
