package tadiran.webnla;

import tadiran.webnla.bean.Group;
import tadiran.webnla.bean.PeriodData;
import tadiran.webnla.synch.SuperGroupsUpdater;
import tadiran.webnla.synch.SuperGroupPeriodsUpdater;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;


/**
 * Store active NLA groups info
 * @author evgeniv
 */
public class SuperGroupPeriods extends AbsCollection
{
    public SuperGroupPeriods() {
        log.info("NLA SuperGroupPeriods collection CREATED");
        collection=new ExtHashMap<Integer,Group>();
        ((ExtHashMap)collection).setBeforeGet(new PeriodGetEvents());
    }

    @Override
    public void onCreateUpdater() {
        log.info("CREATE SuperGroupPeriods updater");
        collectionUpdater=SuperGroupPeriodsUpdater.getInstance();
    }

    private synchronized void addPeriod(Integer id)
    {
        if (!collection.containsKey(id))
        {
            log.debug("NLA SuperGroupPeriods collection ADD period id="+id);
            PeriodData period=new PeriodData(id);
            collection.put(id, period);
            if (collectionUpdater != null) {
                collectionUpdater.forceUpdate();
            }
        }
    }


    public class PeriodGetEvents implements ExtHashMapNotification<Integer>
    {
        @Override
        public void beforeGet (Integer id) {
            addPeriod(id);
        }
    }
}
