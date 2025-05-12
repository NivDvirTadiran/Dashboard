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
public class PeriodsUpdater extends AbsUpdater {
    protected static final int INTERVAL=2000;
    protected static PeriodsUpdater g_inst;
    private GenInfoRequestType reqData;
    private long lastHourlyInfoTime=0;
    private final int hourlyInfoINTERVAL=5*60*1000; //5min


    private PeriodsUpdater(AbsCollection p_collection) {
        super(p_collection);
        log.info("CREATE periods updater");
        reqData = new GenInfoRequestType();
        reqData.setRequestInfoHeader(reqHeader);
    }

    public synchronized static PeriodsUpdater getInstance() {
        if (g_inst==null)
            g_inst=new PeriodsUpdater(ctx.getGroupPeriods());

        return g_inst;
    }
    
    public void forceUpdate() {
        super.forceUpdate();
        lastHourlyInfoTime=0;
    }

    @Override
    protected void onUpdate() throws Exception {
        // get active groups ids
        List<Long> groupIds=this.collection.getActiveIds();
        if (groupIds.isEmpty())
            return;

        // get group queue info
        log.debug("UPDATE PERIOD data, ids="+groupIds);
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
        
        // update daily info
        ReturnArrayDataType[] groupDailyInfoArr = new ReturnArrayDataType[2];
        groupDailyInfoArr[0]=wsEMIS_port.groupDailyBriefInfo(reqData);
        groupDailyInfoArr[1]=wsEMIS_port.groupDailyDetailedInfo(reqData);
        for (ReturnArrayDataType groupDailyInfo: groupDailyInfoArr)
        { // per daily info type
            for (BlockItemType groupPeriodInfo: (List<BlockItemType>)groupDailyInfo.getReturnArrayOfBlocks())
            { // per group
                log.debug("UPDATE group.period("+groupPeriodInfo.getId()+") daily data");
                period=(PeriodData)this.collectionData.get(groupPeriodInfo.getId());
                period.setData("6_3_1_1_1", "Group Name", groupPeriodInfo.getName());
                period.updateData(groupPeriodInfo.getReturnArray());
            }
        }
    }
}
