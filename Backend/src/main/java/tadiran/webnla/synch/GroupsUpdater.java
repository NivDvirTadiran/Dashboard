package tadiran.webnla.synch;

import tadiran.emisweb.*;
import tadiran.webnla.AbsCollection;
import tadiran.webnla.Groups;
import tadiran.webnla.bean.Group;

import java.util.Collections;
import java.util.List;

/**
 * Groups updater task handler
 * @author evgeniv
 */
public class GroupsUpdater extends AbsUpdater 
{
    protected static GroupsUpdater inst;
    private GenInfoRequestType reqData;
    private BriefInfoRequestType reqData2;
    private long lastGroupInfoTime=0;
    private final int GroupInfoINTERVAL=2000;
    private long lastGroupPeriodInfoTime=0;
    private final int GroupPeriodInfoINTERVAL=10000; //10sec

    private GroupsUpdater(AbsCollection p_collection)
    {
        super(p_collection);
        log.info("CREATE new GroupsUpdater");
        reqData = new GenInfoRequestType();
        reqData.setRequestInfoHeader(reqHeader);

        reqData2 = new BriefInfoRequestType();
        reqData2.setRequestInfoHeader(reqHeader);
    }

    public synchronized static GroupsUpdater getInstance()
    {
        if (inst==null)
            inst=new GroupsUpdater(ctx.getGroups());

        return inst;
    }
    
    public void forceUpdate()
    {
        super.forceUpdate();
        lastGroupPeriodInfoTime=0;
        lastGroupInfoTime=0;
    }
    
    @Override
    protected void onUpdate() throws Exception
    {
        log.debug("GroupsUpdater:onUpdate");
        String ticket = reqData.getRequestInfoHeader().getTicket();

        //30-Oct-2022 YR BZ#55845
        if(ticket.isEmpty())
        {
            log.warn("GroupsUpdater:onUpdate TICKET is EMPTY.  Trying reconnect...");
            reconnect();
            return;
        }
        
        List<Long> groupIdsData= reqData.getId();
        // get active groups ids
        List<Long> groupIds=this.collection.getActiveIds();

        log.debug("GroupsUpdater:onUpdate for Ticket " + ticket + ", Group Ids: " + groupIds);

        Group group;
        //get help group data
        if (groupIds.contains(-1L))
        {
            log.debug("GroupsUpdater:onUpdate for help group data");
            group=(Group)(this.collectionData.get(-1));
            group.updateAgentsData((List<DataItemRow>)wsEMIS_port.agentNeedHelp(reqData2).getReturnMatrix());
            //19-Feb-2019 YR BZ#28527
            groupIds.remove(-1L);
        }
        
        if (groupIds.isEmpty())
        {
            log.debug("GroupsUpdater:onUpdate groupIds.isEmpty()");
            return;
        }
        
        if (System.currentTimeMillis()-lastGroupInfoTime >= GroupInfoINTERVAL)
        {
            // get group queue info
            groupIdsData.clear();
            groupIdsData.addAll(groupIds);

            ReturnArrayDataType result[]=new ReturnArrayDataType[3];
            result[0] = wsEMIS_port.groupQueInfo(reqData);		// get group queue info
            if(result[0].getResponseInfoHeader().getErrorCause() == 500)
            {
                log.warn("GroupsUpdater:onUpdate got Error 500.  Trying reconnect...");
                //10-Mar-2020 YR BZ#52018
                reconnect();
//		PageContext ctx = (PageContext) this.getJspContext();
//		String objName=ref;
//                // redirect to select object id
//                HttpServletRequest req=(HttpServletRequest) ctx.getRequest();
//                String url=req.getRequestURI();
//                if (req.getQueryString()!=null)
//                    url+="?"+req.getQueryString();
//
//                String refName[]=ref.split("\\.");
//                log.info("doTag - ctx.forward  /selectObject.jsp?ref=" + refName[0] + "&type=" + type + "&multiSelect=" + this.many + "&params=" + URLEncoder.encode(url));
//                ctx.forward("/selectObject.jsp?ref="+refName[0]+"&type="+type+"&multiSelect="+this.many+"&params="+URLEncoder.encode(url));
                return;
            }
            
            result[1] = wsEMIS_port.groupAgentsInfo(reqData);	// get group agents info
            if (System.currentTimeMillis()-lastGroupPeriodInfoTime >= GroupPeriodInfoINTERVAL) {
                result[2] = wsEMIS_port.groupPeriodInfo(reqData);	// get group period info
                lastGroupPeriodInfoTime=System.currentTimeMillis();
            }

            List<BlockItemType> groupsInfoType[] = new List[result.length];
            for (int i=0; i<groupsInfoType.length; i++) {
                if (result[i] != null)
                    groupsInfoType[i] = (List<BlockItemType>) result[i].getReturnArrayOfBlocks();
            }

            //10-Aug-2022 YR BZ#56641
            //log.info("GroupsUpdater:onUpdate - ServerInfo " + result[i].getResponseInfoHeader().getServersInfo());
            //super.updateServerTime(result[1].getResponseInfoHeader().getServersInfo());
        
            // update groups data
            BlockItemType groupItem;
            for (int i=0; i<groupsInfoType[0].size(); i++)  // per group
            {
                groupItem=groupsInfoType[0].get(i);
                group = (Group)(collectionData.get(groupItem.getId()));

                String out1 = null;
                //String out2 = null;
                try {
                    out1 = new String(groupItem.getName().getBytes("ISO-8859-1"), "UTF-8"); //convertFromUTF8
                }
                catch (java.io.UnsupportedEncodingException e) {
                    log.error("GroupsUpdater.forceUpdate - UnsupportedEncodingException: " + e);
                }

                //group.setData("6_3_1_1_1", "Group Name", LocaleMessage.encode(groupItem.getName()));
                //group.setData("6_3_1_1_1", "Group Name", groupItem.getName());
                group.setData("6_3_1_1_1", "Group Name", out1);
                log.debug("update group(" + group.getId() + "," + out1 + ") data");
                for (int j=0; j<groupsInfoType.length; j++)  //per group function
                {
                    if (groupsInfoType[j]!=null) {
                        groupItem = groupsInfoType[j].get(i);
                        group.updateData((List<DataItemType>) groupItem.getReturnArray());
                    }
                }			
            }

            lastGroupInfoTime=System.currentTimeMillis();
        }

        // get active groups ids
        groupIds=((Groups)this.collection).getActiveAgentIds();
        //19-Feb-2019 YR BZ#28527
        if (groupIds.contains(-1L))
        {
            groupIds.remove(-1L);
        }
        if (groupIds.isEmpty())
            return;

        log.debug("GroupsUpdater - UPDATE GROUPS AGENTS data, ids=" + groupIds + " for Ticket " + ticket);
        groupIdsData.clear();
        groupIdsData.addAll(groupIds);

        //update group agents
        ReturnMatrixDataTypePerGroup result2=wsEMIS_port.agentsPerGroupInfo(reqData);
        List<ReturnDataRowsPerGroup> groupsAgentsInfo= (List<ReturnDataRowsPerGroup>) result2.getReturnDataRowsPerGroup();
        ReturnDataRowsPerGroup groupAgentsItem;
        for (int i=0; i<groupsAgentsInfo.size(); i++)
        {
            groupAgentsItem=groupsAgentsInfo.get(i);
            group=(Group)(this.collectionData.get(groupAgentsItem.getId()));
            log.debug("update group("+group.getId()+") agents data");
            group.updateAgentsData((List<DataItemRow>) groupAgentsItem.getReturnMatrix());
        }
        
        log.debug("GroupsUpdater:onUpdate for Ticket " + ticket + " - Completed");
    }
}
