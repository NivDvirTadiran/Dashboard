package tadiran.webnla.synch;

import tadiran.emisweb.BlockItemType;
import tadiran.webnla.AbsCollection;
import tadiran.emisweb.DataItemRow;
import tadiran.emisweb.GenInfoRequestType;
import tadiran.emisweb.ReturnArrayDataType;
import tadiran.emisweb.ReturnDataRowsPerGroup;
import tadiran.emisweb.ReturnMatrixDataTypePerGroup;
import tadiran.webnla.bean.PeriodData;

import java.util.Collections;
import java.util.List;

/**
 * Agents updater task handler
 * @author evgeniv
 */
public class GroupPeriodsUpdater extends AbsUpdater {
    protected static final int INTERVAL=2000;
    protected static GroupPeriodsUpdater inst;
    private GenInfoRequestType reqData;
    private long lastHourlyInfoTime=0;
    private final int hourlyInfoINTERVAL=5*60*1000; //5min


    private GroupPeriodsUpdater(AbsCollection p_collection)
    {
        super(p_collection);
        log.info("CREATE GroupPeriods updater");
        reqData = new GenInfoRequestType();
        reqData.setRequestInfoHeader(reqHeader);
    }

    public synchronized static GroupPeriodsUpdater getInstance()
    {
        if (inst==null)
            inst=new GroupPeriodsUpdater(ctx.getGroupPeriods());
        return inst;
    }
    
    public void forceUpdate() {
        super.forceUpdate();
        lastHourlyInfoTime=0;
    }

    @Override
    protected void onUpdate() throws Exception
    {
        String ticket = reqData.getRequestInfoHeader().getTicket();

        //30-Oct-2022 YR BZ#55845
        if(ticket.isEmpty())
        {
            log.warn("GroupPeriodsUpdater:onUpdate TICKET is EMPTY.  Trying reconnect...");
            reconnect();
            return;
        }
        
        // get active groups ids
        List<Long> groupIds=this.collection.getActiveIds();
        if (groupIds.isEmpty())
            return;

        // get group queue info
        log.debug("UPDATE PERIOD data, Group Ids="+groupIds);
        List<Long> groupIdsData= reqData.getId();
        groupIdsData.clear();
        groupIdsData.addAll(groupIds);

        PeriodData period;
        // update hourly info
        if (System.currentTimeMillis()-lastHourlyInfoTime >= hourlyInfoINTERVAL)
        {
            ReturnMatrixDataTypePerGroup groupHourlyInfo = wsEMIS_port.groupHourlyInfo(reqData);
            List<ReturnDataRowsPerGroup> returnDataRowsPerGroup = (List<ReturnDataRowsPerGroup>) groupHourlyInfo.getReturnDataRowsPerGroup();
            for(ReturnDataRowsPerGroup groupDataRows: returnDataRowsPerGroup)
            {
                log.debug("UPDATE group.period("+groupDataRows.getId()+") hourly data");
                List<DataItemRow> groupRowsData = (List<DataItemRow>) groupDataRows.getReturnMatrix();
                period=(PeriodData)this.collectionData.get(groupDataRows.getId());
                period.updateHourlyData(groupRowsData);
            }
            lastHourlyInfoTime=System.currentTimeMillis();
        }

        // update Group daily info
        ReturnArrayDataType[] groupDailyInfoArr = new ReturnArrayDataType[2];
        groupDailyInfoArr[0]=wsEMIS_port.groupDailyBriefInfo(reqData);
        
        //12-Sep-2022 YR BZ#56823
        //log.info("GroupPeriodsUpdater:onUpdate - ServerInfo " + groupDailyInfoArr[0].getResponseInfoHeader().getServersInfo());
        super.updateServerTime(groupDailyInfoArr[0].getResponseInfoHeader().getServersInfo());
        
        groupDailyInfoArr[1]=wsEMIS_port.groupDailyDetailedInfo(reqData);
        for (ReturnArrayDataType groupDailyInfo: groupDailyInfoArr) 
        { // per daily info type
            for (BlockItemType groupPeriodInfo: ((List<BlockItemType>)groupDailyInfo.getReturnArrayOfBlocks()))
            { // per group
                log.debug("UPDATE group.period("+groupPeriodInfo.getId()+") daily data");
                period=(PeriodData)this.collectionData.get(groupPeriodInfo.getId());
                period.setData("6_3_1_1_1", "Group Name", groupPeriodInfo.getName());
                period.updateData(((List<BlockItemType>)groupDailyInfo.getReturnArrayOfBlocks()));
            }
        }
        // update SuperGroup daily info
        ReturnArrayDataType supergroupDailyInfo = new ReturnArrayDataType();
        supergroupDailyInfo=wsEMIS_port.superGroupDailyBriefInfo(reqData);
        for (BlockItemType supergroupPeriodInfo: (List<BlockItemType>)supergroupDailyInfo.getReturnArrayOfBlocks())
        { // per group
            log.debug("UPDATE supergroup.period("+supergroupPeriodInfo.getId()+") daily data");
            period=(PeriodData)this.collectionData.get(supergroupPeriodInfo.getId());
            period.setData("6_3_4_1_1", "SuperGroup Name", supergroupPeriodInfo.getName());
            period.updateData(supergroupPeriodInfo.getReturnArray());
        }

    }
}
