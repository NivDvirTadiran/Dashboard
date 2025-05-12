package tadiran.webnla.tag;

import jakarta.servlet.ServletRequest;
import jakarta.servlet.jsp.JspException;
import jakarta.servlet.jsp.PageContext;
import jakarta.servlet.jsp.tagext.SimpleTagSupport;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;

/**
 *
 * @author evgeniv
 */
public class DefineName extends SimpleTagSupport {
	private static Logger log = LogManager.getLogger("NLATag");
	private String name;

    /**
     * Called by the container to invoke this tag. 
     * The implementation of this method is provided by the tag library developer,
     * and handles all tag processing, body iteration, etc.
     */
    @Override
    public void doTag() throws JspException {
		PageContext ctx = (PageContext) this.getJspContext();
		ServletRequest req=ctx.getRequest();

		Integer ind=(Integer)req.getAttribute(name+"Id");
		if (ind==null) ind=1;
		req.setAttribute(name+"Id", ind+1);
		req.setAttribute(name, name+ind);
    }

	public void setName (String name) {
		this.name = name;
	}

}
