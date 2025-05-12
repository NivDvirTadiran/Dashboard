package tadiran.webnla.synch;

import tadiran.webnla.AbsCollection;
import tadiran.webnla.bean.Agent;
import tadiran.emisweb.BriefInfoRequestType;
import tadiran.emisweb.DataItemRow;
import tadiran.emisweb.DataItemType;
import java.util.Iterator;
import java.util.*;

import tadiran.emisweb.ReturnMatrixDataTypeBG;

import java.util.stream.Stream;


/**
 * Agents updater task handler
 * @author evgeniv
 */
public class AgentsUpdater extends AbsUpdater
{
    protected static AgentsUpdater inst;
    private BriefInfoRequestType reqData;

    private AgentsUpdater(AbsCollection p_collection)
    {
        super(p_collection);// <-- agents
        log.info("CREATE new AgentsUpdater");
        reqData = new BriefInfoRequestType();
        reqData.setRequestInfoHeader(reqHeader);
    }

    public synchronized static AgentsUpdater getInstance() 
    {
        if (inst==null)
            inst=new AgentsUpdater(ctx.getAgents());
        return inst;
    }

    @Override
    protected void onUpdate() throws Exception
    {
        log.debug("AgentsUpdater:onUpdate");
        String ticket = reqData.getRequestInfoHeader().getTicket();

        //30-Oct-2022 YR BZ#55845
        if(ticket.isEmpty())
        {
            log.warn("AgentsUpdater:onUpdate TICKET is EMPTY.  Trying reconnect...");
            reconnect();
            return;
        }
        
        log.debug("AgentsUpdater:onUpdate for Ticket " + ticket);
        ReturnMatrixDataTypeBG result = wsEMIS_port.briefAgents(reqData);
        
        if(result.getResponseInfoHeader().getErrorCause() == 500)
        {
            log.warn("AgentsUpdater:onUpdate - got Error 500.  Trying reconnect...");
            //10-Mar-2020 YR BZ#52018
            reconnect();
//            PageContext ctx = (PageContext) this.getJspContext();
//            String objName=ref;
//            // redirect to select object id
//            HttpServletRequest req=(HttpServletRequest) ctx.getRequest();
//            String url=req.getRequestURI();
//            if (req.getQueryString()!=null)
//                url+="?"+req.getQueryString();
//
//            String refName[]=ref.split("\\.");
//            log.info("doTag - ctx.forward  /selectObject.jsp?ref=" + refName[0] + "&type=" + type + "&multiSelect=" + this.many + "&params=" + URLEncoder.encode(url));
//            ctx.forward("/selectObject.jsp?ref="+refName[0]+"&type="+type+"&multiSelect="+this.many+"&params="+URLEncoder.encode(url));
            return;
        }
        
        //10-Aug-2022 YR BZ#56641
        //log.info("AgentsUpdater:onUpdate - ServerInfo " + result.getResponseInfoHeader().getServersInfo());
        super.updateServerTime(result.getResponseInfoHeader().getServersInfo());


        List<DataItemRow> dataItemRow=result.getReturnMatrix();
        Iterator<DataItemRow> iterator=dataItemRow.iterator();
        List<DataItemType> agentData;
        Agent agent;
        while (iterator.hasNext())
        {
            agent = new Agent();
            agentData = iterator.next().getReturnArray();

            agent.updateData(agentData);
            this.collectionData.put(agent.getId(), agent);
        }
        log.debug("AgentsUpdater:onUpdate for Ticket " + ticket + " - Completed");
    }
}
