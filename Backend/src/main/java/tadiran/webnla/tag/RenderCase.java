package tadiran.webnla.tag;

import java.io.IOException;
import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.tagext.BodyTagSupport;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;


/**
 * user-agents render support tag
 * @author evgeniv
 */
public class RenderCase extends BodyTagSupport {

	private static Logger log = LogManager.getLogger("NLATag");
	private String mask;


	@Override
	public int doStartTag() throws JspException {
		log.trace("mask=" + mask);
		String agentName=((jakarta.servlet.http.HttpServletRequest)pageContext.getRequest()).getHeader("user-agent");
		if (agentName!=null && agentName.matches(".*"+mask+".*")) {
			return EVAL_BODY_BUFFERED;
		}
		else {
			return SKIP_BODY;
		}
	}

	@Override
	public int doEndTag () throws JspException {
		try {
			if (bodyContent!=null) {
				String body =bodyContent.getString();
				pageContext.getOut().print(body);
				bodyContent.clearBody();
			}
		}
		catch (IOException ex) {
			log.error(ex);
		}

		return EVAL_PAGE;
	}

	public void setMask (String mask) {
		this.mask = mask;
	}
}
