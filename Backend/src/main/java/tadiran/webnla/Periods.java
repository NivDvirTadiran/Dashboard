package tadiran.webnla;

import tadiran.webnla.bean.Group;
import tadiran.webnla.bean.PeriodData;
import tadiran.webnla.synch.GroupsUpdater;
import tadiran.webnla.synch.PeriodsUpdater;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;


/**
 * Store active NLA groups info
 * @author evgeniv
 */
public class Periods extends AbsCollection {
	public Periods() {
		log.info("NLA periods collection CREATED");
		collection=new ExtHashMap<Integer,Group>();
		((ExtHashMap)collection).setBeforeGet(new PeriodGetEvents());
	}

	@Override
	public void onCreateUpdater() {
		log.info("CREATE periods updater");
		collectionUpdater=PeriodsUpdater.getInstance();
	}

	private synchronized void addPeriod(Integer id) {
		if (!collection.containsKey(id)) {
			log.debug("NLA periods collection ADD period id="+id);
			PeriodData period=new PeriodData(id);
			collection.put(id, period);
            
            if (collectionUpdater != null) {
                collectionUpdater.forceUpdate();
            }
		}
	}


    public class PeriodGetEvents implements ExtHashMapNotification<Integer> {
		@Override
		public void beforeGet (Integer id) {
			addPeriod(id);
		}
	}
}
