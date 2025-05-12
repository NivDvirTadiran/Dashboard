package tadiran.webnla.tag;

import tadiran.webnla.NLAContext;
import java.net.URLEncoder;
import jakarta.servlet.ServletException;
import jakarta.servlet.ServletRequest;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.PageContext;
import jakarta.servlet.jsp.tagext.SimpleTagSupport;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;
import tadiran.webnla.bean.SessionData;


/**
 * retrive NLA object by ref name
 * @author evgeniv
 */
public class RefObject extends SimpleTagSupport
{
    private static Logger log = LogManager.getLogger("NLATag");
    private String ref;
    private String type="";
    private boolean many=false;

    /**
     * Called by the container to invoke this tag.
     * The implementation of this method is provided by the tag library developer,
     * and handles all tag processing, body iteration, etc.
     */
    @Override
    public void doTag () throws JspException 
    {
        PageContext         ctx = (PageContext) this.getJspContext();
        String              objName = ref;
        HttpServletRequest  req = (HttpServletRequest) ctx.getRequest();

        try {
            HttpSession session = req.getSession();
            if(session==null)
            {
                log.warn("RefObject::doTag - Session is NULL" );
            }

            SessionData sessionData = (SessionData)session.getAttribute("sessionData");
            if(sessionData==null)
            {
                //ServletResponse resp = ctx.getResponse();
                HttpServletResponse resp = (HttpServletResponse) ctx.getResponse();
                //log.info("doTag - call resp.sendRedirect to login.jsp");
                //resp.sendRedirect("login.jsp");
                log.warn("RefObject::doTag - sessionData is NULL for Session ID " + session.getId() + " - Forward to LOGIN Page");
                req.getRequestDispatcher("login.jsp").forward(req, resp);
                return;
            }

            if (ref.indexOf(":")==-1) 
            {
                // get object id from url param
                objName=getRefObjName(ref, ctx.getRequest());
                log.debug("RefObject::doTag (Session ID " + req.getSession().getId() + ") - objName: " + objName);
                if (objName==null)
                {
                    // redirect to select object id
                    String url=req.getRequestURI();
                    if (req.getQueryString()!=null)
                        url+="?"+req.getQueryString();

                    String refName[]=ref.split("\\.");
                    log.info("RefObject::doTag - ctx.forward  /selectObject.jsp?ref=" + refName[0] + "&type=" + type + "&multiSelect=" + this.many + "&params=" + URLEncoder.encode(url));
                    ctx.forward("/selectObject.jsp?ref="+refName[0]+"&type="+type+"&multiSelect="+this.many+"&params="+URLEncoder.encode(url));
                    return;
                }
            }
            log.debug("RefObject::doTag - ctx.setAttribute(\"refObjectName\"," + objName  + ")");
            ctx.setAttribute("refObjectName", objName, PageContext.REQUEST_SCOPE);
            if (objName.indexOf(',')==-1) // single object ref
            {
                log.debug("RefObject::doTag - ctx.setAttribute(\"refObject\"," + NLAContext.getInstance().getObject(objName)  + ")");
                ctx.setAttribute("refObject", NLAContext.getInstance().getObject(objName), PageContext.REQUEST_SCOPE);
            }

            getJspBody().invoke(null);
        }
        catch(NullPointerException ex) {
            log.error(ex);
        }
        catch (ServletException ex) {
            log.error(ex);
        }
        catch (java.io.IOException ex) {
            log.error(ex);
        }
        catch (java.lang.IllegalStateException ex) {
            log.error(ex);
        }
    }

    public static String getRefObjName(String ref, ServletRequest req) 
    {
        String refName[]=ref.split("\\.");
        String objId=req.getParameter(refName[0].toLowerCase()+"Id");

        String objName=null;
        if (objId!=null && !"".equals(objId))
        {
            StringBuilder tmpObjName=new StringBuilder();
            for (String id: objId.split(",")) 
            {
                tmpObjName.append(",").append(refName[0]).append(":").append(id);
                if (refName.length>1)
                    tmpObjName.append(".").append(refName[1]);
            }
            objName=tmpObjName.substring(1);
        }

        log.debug("RefObject::getRefObjName for ref: " + ref + ", return: " + objName + ")");
        return objName;
    }

    public void setRef (String mref)
    {
        log.debug("RefObject::setRef mref: " + mref);
        String refName[]=mref.split("\\.");
        if (refName[0].endsWith("s"))
        {
            this.many=true;
            mref=refName[0].substring(0, refName[0].length()-1);
            if (refName.length>1)
                mref+="."+refName[1];
        }
        this.ref = mref;
    }

    public void setType (String val)
    {
        this.type=val.toLowerCase();
    }

    public void setMany(String value)
    {
        if (value.equalsIgnoreCase("true" ))
            this.many = true;
    }
}
