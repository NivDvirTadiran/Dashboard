package tadiran.webnla.synch;

import tadiran.emisweb.BlockItemType;
import tadiran.emisweb.DataItemType;
import tadiran.emisweb.GenInfoRequestType;
import tadiran.webnla.AbsCollection;
import tadiran.webnla.bean.IVRGroup;

import java.util.Collections;
import java.util.Iterator;
import java.util.List;

/**
 * IVRs updater task handler
 * @author evgeniv
 */
public class IVRGrpsUpdater extends AbsUpdater {
    protected static final int INTERVAL=2000;
	protected static IVRGrpsUpdater inst;
	private GenInfoRequestType reqData;

	private IVRGrpsUpdater(AbsCollection p_collection) {
		super(p_collection);
		log.info("CREATE nla IVRGrp updater");

		reqData = new GenInfoRequestType();
		reqData.setRequestInfoHeader(reqHeader);
	}

	public synchronized static IVRGrpsUpdater getInstance() {
		if (inst==null)
			inst=new IVRGrpsUpdater(ctx.getIVRGroups());
		
		return inst;
	}

	@Override
	protected void onUpdate() throws Exception {
		// get active ivr groups ids
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
		IVRGroup ivr;

		// update IVR group data
		iterator=((List) wsEMIS_port.ivrGrpInfo(reqData).getReturnArrayOfBlocks()).iterator();
		while (iterator.hasNext()) {
			ivrStruct=iterator.next();
			ivrData= (List<DataItemType>) ivrStruct.getReturnArray();
			ivr=(IVRGroup)(collectionData.get(ivrStruct.getId()));
			log.debug("UPDATE IVRGrp("+ivr.getName()+") data");
			ivr.updateData(ivrData);
		}

		/*iterator=wsEMIS_port.ivrPortInfo(reqData).getReturnArrayOfBlocks().iterator();
		while (iterator.hasNext()) {
			ivrStruct=iterator.next();
			log.debug("UPDATE IVRPort("+ivrStruct.getId()+") data");
			ivrData=ivrStruct.getReturnArray();
			
			ivr=(IVRGroup)(collectionData.get(ivrStruct.getId()));
			ivr.addPortsData(ivrData);
		}*/
	}
}
