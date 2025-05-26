package tadiran.webnla.accrealtime.services;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import tadiran.webnla.bean.SessionData;

import jakarta.servlet.http.HttpSession;
import jakarta.servlet.http.HttpSessionEvent;
import jakarta.servlet.http.HttpSessionListener;


/**
 * SessionsListener
 *
 * @author evgeniv
 */
public class SessionsListener implements HttpSessionListener
{
    private static final Logger log = LogManager.getLogger("NLAContext");

    public SessionsListener() {
    }

    public void sessionCreated(HttpSessionEvent se) {
        se.getSession().setMaxInactiveInterval(45);
    }

    public void sessionDestroyed(HttpSessionEvent se) {
        HttpSession session = se.getSession();
        SessionData sessData = (SessionData)session.getAttribute("sessionData");
        if (sessData != null) {
            log.info("LOGOUT session expired, userName=" + session.getAttribute("userName"));
            sessData.logout(true);
            sessData = null;
            session.invalidate();
        }

    }
}
