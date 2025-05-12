package tadiran.webnla;

import tadiran.webnla.bean.AbsDataClass;
import tadiran.webnla.synch.AbsUpdater;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;



/**
 * abstract objects collection
 * @author evgeniv
 */
public abstract class AbsCollection
{
    protected static final Logger log = LogManager.getLogger("NLAContext");

    protected HashMap collection;
    protected AbsUpdater collectionUpdater;
    protected int collectionUpdaterInitialized=0;
    protected long lastAccessTime=0;


    public abstract void onCreateUpdater();

    /**
     * get object data,
     * (also create updated if required)
     * @return HashMap
     */
    public HashMap getData()
    {
        if (collectionUpdaterInitialized==0)
        {
            collectionUpdaterInitialized=1;
            if (collection==null)
                collection=new HashMap();

            onCreateUpdater();
            log.debug("NLA objects collection "+this+" CREATED");
        }
        if (collectionUpdaterInitialized==1)
        {
            synchronized(collection)
            {
                try {
                    log.trace("initial wait for collection data update...");
                    collection.wait();
                } catch (InterruptedException ex) {
                }
            }
            collectionUpdaterInitialized=2;
        }
        return getCollection();
    }

    public boolean isInitialized() {
        return collectionUpdaterInitialized==2;
    }

    /**
     * get ref to collection
     * @return HashMap
     */
    public HashMap getCollection() {
        lastAccessTime=System.currentTimeMillis();
        return collection;
    }

    /**
     * test collection access is expired timeout
     * @return
     */
    public boolean isExpired() {
        return (System.currentTimeMillis()-lastAccessTime)>AbsDataClass.ACCESS_TIMEOUT;
    }

    /**
     * get active (not expired) objects ids
     * @return List
     */
    public List<Long> getActiveIds()
    {
        List ids=new Vector();
        AbsDataClass obj;
        Iterator iterator=collection.values().iterator();
        while(iterator.hasNext())
        {
            obj=(AbsDataClass)iterator.next();
            if (!obj.isExpired())
                ids.add(Long.valueOf(obj.getId()));
        }
        log.debug("collection " + this.getClass() + " GET active Ids=" + ids);
        return ids;
    }

    @Override
    protected void finalize() throws Throwable
    {
        log.debug("NLA objects collection " + this + " DESTROY");
        if (collectionUpdater!=null)
            collectionUpdater.cancel();
	
        collectionUpdater=null;
        collection=null;
        super.finalize();
    }
}
