package tadiran.webnla;

import tadiran.webnla.bean.IVRGroup;
import tadiran.webnla.synch.IVRGrpsUpdater;


/**
 * Store active NLA IVRs info
 * @author evgeniv
 */
public class IVRGroups extends AbsCollection {
	public IVRGroups() {
		log.info("NLA IVRGrp collection CREATED");
		collection=new ExtHashMap<Integer,IVRGroup>();
		((ExtHashMap)collection).setBeforeGet(new IVRGetEvents());
	}

	@Override
	public void onCreateUpdater() {
		log.info("CREATE IVRGrp updater");
		collectionUpdater=IVRGrpsUpdater.getInstance();
	}

	private synchronized void addIVRGrp(Integer id) {
		if (!collection.containsKey(id)) {
			log.debug("NLA IVRGrp collection ADD IVRGrp id="+id);
			IVRGroup ivr=new IVRGroup(id);
			collection.put(id, ivr);
            
            collectionUpdater.forceUpdate();
		}
	}

	public class IVRGetEvents implements ExtHashMapNotification<Integer> {
		@Override
		public void beforeGet (Integer id) {
			addIVRGrp(id);
		}
	}
}
