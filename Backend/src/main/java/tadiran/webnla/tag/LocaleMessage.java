package tadiran.webnla.tag;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.HashMap;
import java.util.Properties;
import jakarta.servlet.ServletContext;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.JspWriter;
import jakarta.servlet.jsp.PageContext;
import jakarta.servlet.jsp.tagext.SimpleTagSupport;
//import java.util.Base64;


/**
 *
 * @author evgeniv
 */
public class LocaleMessage extends SimpleTagSupport {
    private static HashMap<String, Properties> localeMessages;
    private static String captionOnCurrentIntervalSTR = "caption.OnCurrentInterval";
    //16-Jun-2019 YR BZ#50047
   // public static String langName;
    private String message;

    public static void init(ServletContext ctx) {
        localeMessages = (HashMap<String, Properties>) ctx.getAttribute("localeMessages");
    }
    
   

    /**
     * Called by the container to invoke this tag.
     * The implementation of this method is provided by the tag library developer,
     * and handles all tag processing, body iteration, etc.
     */
    @Override
    public void doTag () throws JspException, IOException {
        PageContext pageContext=(PageContext)this.getJspContext();
        HttpServletRequest request=(HttpServletRequest)pageContext.getRequest();
        String langName = (String)request.getSession().getAttribute("Accept-Language");
        message=getMessage(message, langName);
        JspWriter out = pageContext.getOut();
        out.print(message);
       // langName = (String)request.getSession().getAttribute("Accept-Language");
    }
        
    public static String encode(String value) {
        try {
            //byte[] bytes = value.getBytes("UTF-8");
            //String encoded = Base64.getEncoder().encodeToString(bytes);
            //byte[] decoded = Base64.getDecoder().decode(value);
            //value = new String(decoded);
                
            value=new String(value.getBytes("ISO-8859-1"));
        }
        catch (UnsupportedEncodingException ex) {
        }
        return value;
    }

    public static String getMessage(String p_message, String locale) {
        String message=p_message;
        if(p_message==null || p_message.isEmpty())
            return "";
        
        //16-Jun-2019 YR BZ#50047
        if (localeMessages != null && !localeMessages.isEmpty()) {

            if (locale != null)
                if (locale.length() > 2)
                    locale = locale.substring(0, 2);
            
            Properties messages = (Properties) localeMessages.get("_" + locale);
            if (messages == null)
                messages = (Properties) localeMessages.get("_en");

            if(p_message.contains(captionOnCurrentIntervalSTR))
            {
                String captionOnCurrentIntervalTrans = messages.getProperty(captionOnCurrentIntervalSTR);
                if(captionOnCurrentIntervalTrans != null)
                    message = p_message.replace(captionOnCurrentIntervalSTR, captionOnCurrentIntervalTrans);
            }
            else if(p_message.equals("Forced Rel"))
            {
                message = messages.getProperty("ForcedRelease");
            }
            else if(p_message.equals("Forced Busy"))
            {
                message = messages.getProperty("ForcedBusy");
            }
            else
                message = messages.getProperty(p_message);
            
            if (message==null)
                message=p_message;
            }
            return message;
	}

	public void setMessage (String message) {
            this.message = message;
	}
}
