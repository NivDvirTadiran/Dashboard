package tadiran.webnla;

import tadiran.webnla.bean.Group;
import tadiran.webnla.synch.GroupsUpdater;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;


/**
 * Store active NLA groups info
 * @author evgeniv
 */
public class Groups extends AbsCollection {
	public Groups() {
		log.info("NLA groups collection CREATED");
		collection=new ExtHashMap<Integer,Group>();
		((ExtHashMap)collection).setBeforeGet(new GroupGetEvents());
	}

	@Override
	public void onCreateUpdater() {
		log.info("CREATE groups updater");
		collectionUpdater=GroupsUpdater.getInstance();
	}

	private synchronized void addGroup(Integer id) {
		if (!collection.containsKey(id)) {
			log.debug("NLA groups collection ADD group id="+id);
			Group group=new Group(id);
			collection.put(id, group);
            
            collectionUpdater.forceUpdate();
		}
	}

	// get id list of group's agents
	public List<Long> getActiveAgentIds() {
		List ids=new Vector();
		Group group;
		Iterator iterator=collection.values().iterator();
		while(iterator.hasNext()) {
			group=(Group)iterator.next();
			if (!group.isAgentsExpired())
				ids.add(Long.valueOf(group.getId()));
		}
		log.trace("collection "+this+" GET active agent in group Ids="+ids);
		return ids;
	}

	public class GroupGetEvents implements ExtHashMapNotification<Integer> {
		@Override
		public void beforeGet (Integer id) {
			addGroup(id);
		}
	}
}
