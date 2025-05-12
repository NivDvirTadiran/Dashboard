package tadiran.webnla;

import tadiran.webnla.synch.DNISsUpdater;


/**
 * Store active NLA DNISs info
 * @author evgeniv
 */
public class DNISs extends AbsCollection {
	@Override
	public void onCreateUpdater() {
		log.info("CREATE dnis updater");
		collectionUpdater=DNISsUpdater.getInstance();
	}
}
