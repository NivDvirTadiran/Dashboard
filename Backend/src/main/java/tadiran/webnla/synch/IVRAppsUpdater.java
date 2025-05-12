package tadiran.webnla.synch;

import tadiran.emisweb.BlockItemType;
import tadiran.emisweb.DataItemType;
import tadiran.emisweb.GenInfoRequestType;
import tadiran.webnla.AbsCollection;
import tadiran.webnla.bean.IVRApp;

import java.util.Collections;
import java.util.Iterator;
import java.util.List;

/**
 * IVRs updater task handler
 * @author evgeniv
 */
public class IVRAppsUpdater extends AbsUpdater {
    protected static final int INTERVAL=2000;
	protected static IVRAppsUpdater inst;
	private GenInfoRequestType reqData;

	private IVRAppsUpdater(AbsCollection p_collection) {
		super(p_collection);
		log.info("CREATE nla IVR App updater");

		reqData = new GenInfoRequestType();
		reqData.setRequestInfoHeader(reqHeader);
	}

	public synchronized static IVRAppsUpdater getInstance() {
		if (inst==null)
			inst=new IVRAppsUpdater(ctx.getIVRApps());
		
		return inst;
	}

	@Override
	protected void onUpdate() throws Exception {
		// get active ivr app ids
		List<Long> activeIVRIds=this.collection.getActiveIds();
		if (activeIVRIds.isEmpty())
			return;

		// get group queue info
		List<Long> requestIVRIds= reqData.getId();
		requestIVRIds.clear();
		requestIVRIds.addAll(activeIVRIds);

		Iterator<BlockItemType> iterator;
		List<DataItemType> ivrData;
		BlockItemType ivrStruct;
		IVRApp ivr;

		// update voice IVR data
		iterator=((List<BlockItemType>)wsEMIS_port.ivrApplInfo(reqData).getReturnArrayOfBlocks()).iterator();
		while (iterator.hasNext()) {
			ivrStruct=iterator.next();
			log.debug("UPDATE IVRApp("+ivrStruct.getId()+") data");
			ivrData= (List<DataItemType>) ivrStruct.getReturnArray();

			ivr=(IVRApp)(collectionData.get(ivrStruct.getId()));
			ivr.updateData(ivrData);
		}
	}
}
