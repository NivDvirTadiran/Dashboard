package tadiran.webnla;

import tadiran.webnla.bean.Group;
import tadiran.webnla.bean.PeriodData;
import tadiran.webnla.synch.GroupsUpdater;
import tadiran.webnla.synch.GroupPeriodsUpdater;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;


/**
 * Store active NLA groups info
 * @author evgeniv
 */
public class GroupPeriods extends AbsCollection
{
    public GroupPeriods() {
        log.info("NLA GroupPeriods collection CREATED");
        collection=new ExtHashMap<Integer,Group>();
        ((ExtHashMap)collection).setBeforeGet(new PeriodGetEvents());
    }

    @Override
    public void onCreateUpdater() {
        log.info("CREATE GroupPeriods updater");
        collectionUpdater=GroupPeriodsUpdater.getInstance();
    }

    private synchronized void addPeriod(Integer id)
    {
        if (!collection.containsKey(id))
        {
            log.debug("NLA GroupPeriods collection ADD period id="+id);
            PeriodData period=new PeriodData(id);
            collection.put(id, period);
            collectionUpdater.forceUpdate();
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
