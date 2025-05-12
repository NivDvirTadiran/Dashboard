package tadiran.webnla;

import tadiran.webnla.bean.SuperGroup;
import tadiran.webnla.synch.SuperGroupsUpdater;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;


/**
 * Store active NLA groups info
 * @author evgeniv
 */
public class SuperGroups extends AbsCollection {
	public SuperGroups() {
		log.info("NLA SuperGroups collection CREATED");
		collection=new ExtHashMap<Integer,SuperGroup>();
		((ExtHashMap)collection).setBeforeGet(new SuperGroupGetEvents());
	}

	@Override
	public void onCreateUpdater() {
		log.info("CREATE SuperGroups updater");
		collectionUpdater=SuperGroupsUpdater.getInstance();
	}

	private synchronized void addSuperGroup(Integer id) {
		if (!collection.containsKey(id)) {
			log.debug("NLA SuperGroups collection ADD super group id="+id);
			SuperGroup superGroup=new SuperGroup(id);
			collection.put(id, superGroup);
            
            collectionUpdater.forceUpdate();
		}
	}

	// get id list of SuperGroup's groups
	public List<Long> getActiveGroupIds() {
		List ids=new Vector();
		SuperGroup superGroup;
		Iterator iterator=collection.values().iterator();
		while(iterator.hasNext()) {
			superGroup=(SuperGroup)iterator.next();
			if (!superGroup.isAgentsExpired())
				ids.add(Long.valueOf(superGroup.getId()));
		}
		log.trace("collection "+this+" GET active groups in SuperGroup Ids="+ids);
		return ids;
	}

	public class SuperGroupGetEvents implements ExtHashMapNotification<Integer> {
		@Override
		public void beforeGet (Integer id) {
			addSuperGroup(id);
		}
	}
}
