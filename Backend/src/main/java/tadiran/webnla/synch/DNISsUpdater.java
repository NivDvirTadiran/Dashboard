package tadiran.webnla.synch;

import tadiran.emisweb.*;
import tadiran.webnla.AbsCollection;
import tadiran.webnla.bean.DNIS;
import java.util.Iterator;
import java.util.List;

/**
 * DNISs updater task handler
 * @author evgeniv
 */
public class DNISsUpdater extends AbsUpdater {
    protected static final int INTERVAL=2000;
    protected static DNISsUpdater inst;
    private BriefInfoRequestType reqData;

    private DNISsUpdater(AbsCollection p_collection)
    {
        super(p_collection);
        log.info("CREATE new DNISsUpdater");
        reqData = new BriefInfoRequestType();
        reqData.setRequestInfoHeader(reqHeader);
    }

    public synchronized static DNISsUpdater getInstance()
    {
        if (inst==null)
            inst=new DNISsUpdater(ctx.getDNISs());
	
            return inst;
    }

    @Override
    protected void onUpdate() throws Exception 
    {
        log.debug("DNISsUpdater:onUpdate");

        Iterator<DataItemRow> iterator;
        List<DataItemType> dnisData;
        DNIS dnis;

        // update voice dnis data
        iterator=((List<DataItemRow>) wsEMIS_port.briefVoiceDNIS(reqData).getReturnMatrix()).iterator();
        while (iterator.hasNext())
        {
            dnis=new DNIS();
            dnisData= (List<DataItemType>) iterator.next().getReturnArray();

            dnis.updateData(dnisData);
            this.collectionData.put(dnis.getId(), dnis);
        }

        log.debug("DNISsUpdater:onUpdate - Completed");
    }
}
