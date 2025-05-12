package tadiran.webnla.synch;

import tadiran.emisweb.BlockItemType;
import tadiran.webnla.AbsCollection;
//import tadiran.webnla.bean.Agent;
//import tadiran.emisweb.BriefInfoRequestType;
import tadiran.emisweb.DataItemRow;
//import tadiran.emisweb.DataItemType;
import tadiran.emisweb.GenInfoRequestType;
import tadiran.emisweb.ReturnArrayDataType;
import tadiran.emisweb.ReturnDataRowsPerSuperGroup;
import tadiran.emisweb.ReturnMatrixDataTypePerSuperGroup;
//import tadiran.webnla.bean.Group;
import tadiran.webnla.bean.PeriodData;
//import java.util.Iterator;
import java.util.Collections;
import java.util.List;

/**
 * Agents updater task handler
 * @author evgeniv
 */
public class SuperGroupPeriodsUpdater extends AbsUpdater {
    protected static final int INTERVAL=2000;
    protected static SuperGroupPeriodsUpdater inst;
    private GenInfoRequestType reqData;
    private long lastHourlyInfoTime=0;
    private final int hourlyInfoINTERVAL=5*60*1000; //5min


    private SuperGroupPeriodsUpdater(AbsCollection p_collection)
    {
        super(p_collection);
        log.info("CREATE SuperGroupPeriods updater");
        reqData = new GenInfoRequestType();
        reqData.setRequestInfoHeader(reqHeader);
    }

    public synchronized static SuperGroupPeriodsUpdater getInstance()
    {
        if (inst==null)
            inst=new SuperGroupPeriodsUpdater(ctx.getSuperGroupPeriods());
        return inst;
    }
    
    public void forceUpdate() {
        super.forceUpdate();
        lastHourlyInfoTime=0;
    }

    @Override
    protected void onUpdate() throws Exception
    {
        // get active super groups ids
        List<Long> supergroupIds=this.collection.getActiveIds();
        if (supergroupIds.isEmpty())
                return;

        // get group queue info
        log.debug("UPDATE PERIOD data, SuperGroup Ids="+supergroupIds);
        List<Long> superGroupIdsData= reqData.getId();
        superGroupIdsData.clear();
        superGroupIdsData.addAll(supergroupIds);

        PeriodData period;
        // update hourly info
        if (System.currentTimeMillis()-lastHourlyInfoTime >= hourlyInfoINTERVAL)
        {
            ReturnMatrixDataTypePerSuperGroup superGroupHourlyInfo = wsEMIS_port.superGroupHourlyInfo(reqData);
            List<ReturnDataRowsPerSuperGroup> returnDataRowsPerSuperGroup = (List<ReturnDataRowsPerSuperGroup>) superGroupHourlyInfo.getReturnDataRowsPerSuperGroup();
            for(ReturnDataRowsPerSuperGroup superGroupDataRows: returnDataRowsPerSuperGroup)
            {
                log.debug("UPDATE SuperGroup.Period("+superGroupDataRows.getId()+") hourly data");
                List<DataItemRow> superGroupRowsData = (List<DataItemRow>) superGroupDataRows.getReturnMatrix();
                period=(PeriodData)this.collectionData.get(superGroupDataRows.getId());
                period.updateHourlyData(superGroupRowsData);
            }
            lastHourlyInfoTime=System.currentTimeMillis();
        }

        // update SuperGroup daily info
        ReturnArrayDataType supergroupDailyInfo = new ReturnArrayDataType();
        supergroupDailyInfo=wsEMIS_port.superGroupDailyBriefInfo(reqData);

        //12-Sep-2022 YR BZ#56823
        //log.info("GroupPeriodsUpdater:onUpdate - ServerInfo " + groupDailyInfoArr[0].getResponseInfoHeader().getServersInfo());
        super.updateServerTime(supergroupDailyInfo.getResponseInfoHeader().getServersInfo());
        
        for (BlockItemType supergroupPeriodInfo: ((List<BlockItemType>) supergroupDailyInfo.getReturnArrayOfBlocks()))
        { // per group
            log.debug("UPDATE supergroup.period("+supergroupPeriodInfo.getId()+") daily data");
            period=(PeriodData)this.collectionData.get(supergroupPeriodInfo.getId());
            period.setData("6_3_4_1_1", "SuperGroup Name", supergroupPeriodInfo.getName());
            period.updateData(((List<BlockItemType>) supergroupDailyInfo.getReturnArrayOfBlocks()));
        }

    }
}
