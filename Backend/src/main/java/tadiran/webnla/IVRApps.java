package tadiran.webnla;

import tadiran.webnla.bean.IVRApp;
import tadiran.webnla.synch.IVRAppsUpdater;


/**
 * Store active NLA IVRs info
 * @author evgeniv
 */
public class IVRApps extends AbsCollection {
	public IVRApps() {
		log.info("NLA IVRApp collection CREATED");
		collection=new ExtHashMap<Integer,IVRApp>();
		((ExtHashMap<?, ?>)collection).setBeforeGet(new IVRGetEvents());
	}

	@Override
	public void onCreateUpdater() {
		log.info("CREATE IVRApp updater");
		collectionUpdater=IVRAppsUpdater.getInstance();
	}

	private synchronized void addIVRApp(Integer id) {
		if (!collection.containsKey(id)) {
			log.debug("NLA IVR collection ADD IVRApp id="+id);
			IVRApp ivr=new IVRApp(id);
			collection.put(id, ivr);
            
            if (collectionUpdater != null) {
                collectionUpdater.forceUpdate();
            }
		}
	}

	public class IVRGetEvents implements ExtHashMapNotification<Integer> {
		@Override
		public void beforeGet (Integer id) {
			addIVRApp(id);
		}
	}
}
