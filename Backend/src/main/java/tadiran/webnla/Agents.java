package tadiran.webnla;

import tadiran.webnla.synch.AgentsUpdater;


/**
 * Store active NLA agents info
 * @author evgeniv
 */
public class Agents extends AbsCollection {
	@Override
	public void onCreateUpdater() {
		log.info("CREATE agents updater");
		collectionUpdater=AgentsUpdater.getInstance();
	}
}
