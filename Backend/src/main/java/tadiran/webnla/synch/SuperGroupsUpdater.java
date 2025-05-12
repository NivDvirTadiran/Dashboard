package tadiran.webnla.synch;

import tadiran.webnla.AbsCollection;
import tadiran.webnla.bean.SuperGroup;
import tadiran.emisweb.BlockItemType;
import tadiran.emisweb.BriefInfoRequestType;
import tadiran.emisweb.GenInfoRequestType;
import tadiran.emisweb.ReturnArrayDataType;

import java.util.Collections;
import java.util.List;

/**
 * Groups updater task handler
 * @author evgeniv
 */
public class SuperGroupsUpdater extends AbsUpdater 
{
    protected static SuperGroupsUpdater inst;
    private GenInfoRequestType reqData;
    private BriefInfoRequestType reqData2;
    private long lastSuperGroupInfoTime=0;
    private final int SuperGroupInfoINTERVAL=2000;
    private long lastSuperGroupPeriodInfoTime=0;
    private final int SuperGroupPeriodInfoINTERVAL=10000; //10sec

    private SuperGroupsUpdater(AbsCollection p_collection)
    {
        super(p_collection);
        log.info("CREATE new SuperGroupsUpdater");
        reqData = new GenInfoRequestType();
        reqData.setRequestInfoHeader(reqHeader);

        reqData2 = new BriefInfoRequestType();
        reqData2.setRequestInfoHeader(reqHeader);
    }

    public synchronized static SuperGroupsUpdater getInstance()
    {
        if (inst==null)
            inst=new SuperGroupsUpdater(ctx.getSuperGroups());

        return inst;
    }
    
    public void forceUpdate()
    {
        super.forceUpdate();
        lastSuperGroupPeriodInfoTime=0;
        lastSuperGroupInfoTime=0;
    }
    
    @Override
    protected void onUpdate() throws Exception
    {
        log.debug("SuperGroupsUpdater:onUpdate");
        String ticket = reqData.getRequestInfoHeader().getTicket();
        
        //30-Oct-2022 YR BZ#55845
        if(ticket.isEmpty())
        {
            log.warn("SuperGroupsUpdater:onUpdate TICKET is EMPTY.  Trying reconnect...");
            reconnect();
            return;
        }
        
        List<Long> superGroupIdsData= reqData.getId();
        SuperGroup superGroup;

        // get active groups ids
        List<Long> superGroupIds=this.collection.getActiveIds();

        log.debug("SuperGroupsUpdater:onUpdate for Ticket " + ticket + ", Super Group Ids: " + superGroupIds);
        
        if (superGroupIds.isEmpty())
        {
            log.debug("SuperGroupsUpdater:onUpdate superGroupIds.isEmpty()");
            return;
        }

        if (System.currentTimeMillis()-lastSuperGroupInfoTime >= SuperGroupInfoINTERVAL)
        {
            // get group queue info
            superGroupIdsData.clear();
            superGroupIdsData.addAll(superGroupIds);

            ReturnArrayDataType result[]=new ReturnArrayDataType[2];
            result[0] = wsEMIS_port.superGroupInfo(reqData);		// get group queue info
            if(result[0].getResponseInfoHeader().getErrorCause() == 500)
            {
                log.warn("SuperGroupsUpdater:onUpdate got Error 500.  Trying reconnect...");
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
            
//            result[1] = wsEMIS_port.groupAgentsInfo(reqData);	// get group agents info
//            if (System.currentTimeMillis()-lastGroupPeriodInfoTime >= GroupPeriodInfoINTERVAL) {
//                result[2] = wsEMIS_port.groupPeriodInfo(reqData);	// get group period info
//                lastGroupPeriodInfoTime=System.currentTimeMillis();
//            }


            //10-Aug-2022 YR BZ#56641
            //log.info("SuperGroupsUpdater:onUpdate - ServerInfo " + result[0].getResponseInfoHeader().getServersInfo());
            super.updateServerTime(result[0].getResponseInfoHeader().getServersInfo());
        
            List<BlockItemType> superGroupsInfoType[] = new List[result.length];
            for (int i=0; i<superGroupsInfoType.length; i++) {
                if (result[i] != null)
                    superGroupsInfoType[i] = (List<BlockItemType>) result[i].getReturnArrayOfBlocks();
            }

            // update super groups data
            BlockItemType superGroupItem;
            for (int i=0; i<superGroupsInfoType[0].size(); i++)  // per super group
            {
                superGroupItem=superGroupsInfoType[0].get(i);
                superGroup = (SuperGroup)(collectionData.get(superGroupItem.getId()));

                String out1 = null;
                //String out2 = null;
                try {
                    out1 = new String(superGroupItem.getName().getBytes("ISO-8859-1"), "UTF-8"); //convertFromUTF8
                }
                catch (java.io.UnsupportedEncodingException e) {
                    log.error("GroupsUpdater.forceUpdate - UnsupportedEncodingException: " + e);
                }

                //group.setData("6_3_1_1_1", "Group Name", LocaleMessage.encode(groupItem.getName()));
                //group.setData("6_3_1_1_1", "Group Name", groupItem.getName());
                superGroup.setData("6_3_4_1_1", "SuperGroup Name", out1);
                log.debug("update super group(" + superGroup.getId() + "," + out1 + ") data");
                for (int j=0; j<superGroupsInfoType.length; j++)  //per group function
                {
                    if (superGroupsInfoType[j]!=null) {
                        superGroupItem = superGroupsInfoType[j].get(i);
                        superGroup.updateData(superGroupItem.getReturnArray());
                    }
                }			
            }
        }



//        Iterator<BlockItemType> iterator;
//        List<DataItemType> superGroupData;
//        SuperGroup super_group;
//
//        // update Super Group data
//        ReturnArrayDataType result = new ReturnArrayDataType();
//        iterator = wsEMIS_port.superGroupInfo(reqData).getReturnArrayOfBlocks().iterator();
//        while (iterator.hasNext())
//        {
//            super_group = new SuperGroup();
//            superGroupData=iterator.next().getReturnArray();
//
//            super_group.updateData(superGroupData);
//            this.collectionData.put(super_group.getId(), super_group);
//        }
        
        log.debug("SuperGroupsUpdater:onUpdate - Completed");
    }
}
