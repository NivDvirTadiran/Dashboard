/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tadiran.webnla;

/**
 *
 * @author a180042
 */
import static tadiran.webnla.AbsCollection.log;
import tadiran.webnla.bean.RCode;
import tadiran.webnla.synch.IVRAppsUpdater;


/**
 * Store active NLA RCodes info
 * @author evgeniv
 */
public class RCodes extends AbsCollection {
	public RCodes() {
		log.info("NLA RCodes collection CREATED");
		collection=new ExtHashMap<Integer,RCode>();
		((ExtHashMap)collection).setBeforeGet(new RCodeGetEvents());
	}

	@Override
	public void onCreateUpdater() {
		//log.info("CREATE RCode updater");
		//collectionUpdater=RCodesUpdater.getInstance();
	}

	private synchronized void addRCode(Integer id) {
		if (!collection.containsKey(id)) {
			log.debug("NLA RCodes collection ADD RCode id="+id);
			RCode rcode=new RCode(id);
			collection.put(id, rcode);
            
            collectionUpdater.forceUpdate();
		}
	}

	public class RCodeGetEvents implements ExtHashMapNotification<Integer> {
		@Override
		public void beforeGet (Integer id) {
			addRCode(id);
		}
	}
}
