package tadiran.webnla.bean;

import tadiran.emisweb.DataItemType;
import tadiran.webnla.tag.LocaleMessage;

import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;

/**
 * Abstract NLA object
 * @author evgeniv
 */
public abstract class AbsDataClass {
	public static final int ACCESS_TIMEOUT=1000*5; // 30sec
	protected Logger log;
	protected HashMap<String,DataItem> data;
	protected int id;
	protected long lastAccessTime;


	protected AbsDataClass(int id) {
		log = LogManager.getLogger("NLAData");
		log.debug("creating NEW nla object: objId="+id);
		data=new HashMap(25, 0.5f);
		this.id=id;
		lastAccessTime=System.currentTimeMillis();
	}

	/**
	 * get data
	 * @return HashMap<String,DataItem>
	 */
	public HashMap<String,DataItem> getData() {
		lastAccessTime=System.currentTimeMillis();
		return this.data;
	}

	/**
	 * get object class name
	 * @return String
	 */
	public String getObjectInfo() {
		return this.getClass().getSimpleName();
	}

	public int getId() {
		return id;
	}

	/**
	 * get object name with id
	 * @return String
	 */
	public String getName() {
		return getObjectInfo()+":"+id;
	}

	abstract public String getNum();
    
    protected int getAccessTimeout() {
        return this.ACCESS_TIMEOUT;
    }

	/**
	 * check object expired, according last access time interval
	 * @return
	 */
	public boolean isExpired() {
		return (System.currentTimeMillis()-lastAccessTime) > getAccessTimeout();
	}

	/**
	 * set object data by key
	 * @param String - itemKey
	 * @param String - itemName
	 * @param String - itemValue
	 */
	public void setData(String itemKey, String itemName, Object itemValue) {
		DataItem itemData=new DataItem(itemKey, itemName, itemValue);
		data.put(itemKey, itemData);
	}

	/**
	 * get object data by key
	 * @param String - key
	 * @return Object
	 */
	public Object getDataValue(String itemKey) {
		DataItem itemData=data.get(itemKey);
		if (itemData!=null)
			return itemData.value;
//		else
//			if (!data.containsKey(itemKey)) {
//				synchronized(this) {
//					try {
//						log.debug("wait new data update for "+itemKey+" ...");
//						this.wait(5000);
//					}
//					catch (InterruptedException ex) {
//					}
//				}
//				itemData=data.get(itemKey);
//				if (itemData!=null)
//					return itemData.value;
//			}

		return "";
	}

	/**
	 * get ers_id name
	 * @param String ers_id
	 * @param HttpServletRequest (or null)
	 * @return String - ers name
	 */
	public String getDataName(String itemKey, String langName) {
		String name=LocaleMessage.getMessage(itemKey, langName);
		if (name.equals(itemKey))
			name=data.get(itemKey).getName();

		return name;
	}

	public DataItem getData(String itemKey) {        
            return data.get(itemKey);
	}
        
	/**
	 * Update NLA Class's Properties
	 * @param List<DataItemType>
	 */
	public void updateData(List items) {
		log.debug("updateData - Started");
		boolean isInitialized=(this.data.size()<=5);
		
		Iterator<DataItemType> iterator = items.iterator();
		DataItemType item;
		String ersKey;
		while (iterator.hasNext()) {
			item= iterator.next();
			ersKey=item.getErsid().substring(1);
			this.data.put(ersKey, new DataItem(ersKey, item.getErsname(), Long.parseLong(item.getValue()), item.getBgcolor()));
		}

		if (isInitialized) {
			log.debug("data initialized");
			synchronized(this) {
				this.notifyAll();
			}
		}
		log.debug("updateData - Ended");
	}

	protected void updateStrData(List items) {
            try
            {
		log.trace("update str data");
		boolean isInitialized=(this.data.size()<=5);
		
		Iterator<DataItemType> iterator = items.iterator();
		DataItemType item;
		String ersKey;
                String value;
                String X = "0";
		while (iterator.hasNext()) {
                    item= iterator.next();
                    ersKey=item.getErsid();
                    if (ersKey.startsWith("f")) {
                        ersKey=ersKey.substring(1);
                    }
            
                    log.debug("update str data <" + ersKey + ":" + item.getValue() + ">");
                
                    if(item.getErsname().equals("ALL"))
                    {
                        String [] s_row;
                        s_row = item.getValue().split(",");
                        int n_rows = s_row.length;
                        //Long bg_color =  new Long(16777215);
                        for (int i = 0; i < n_rows; i++) 
                        {
                            Long bg_color =  new Long(16777215);
                            String [] s_item = s_row[i].split(";");
                            //if(s_item[0].equals("12"))                            
                            //    bg_color = Color.MAGENTA.toString();//"16777215";
                            //else
                            //    bg_color.get = Color.MAGENTA.toString();//s_item[0];
                            if ( !(s_item[0].equals("0")) )
                            {
                                bg_color =  Long.valueOf(s_item[0]);
                            }
                            if (s_item.length < 4)
                            {
                                if((s_item[2].equals("Release Code")))
                                    this.data.put(s_item[1].substring(1), new DataItem(s_item[1].substring(1), s_item[2], "" , bg_color));
                                else
                                    this.data.put(s_item[1].substring(1), new DataItem(s_item[1].substring(1), s_item[2],X , bg_color));
                            }
                            else
                            {                                
                               this.data.put(s_item[1].substring(1), new DataItem(s_item[1].substring(1), s_item[2], s_item[3] , bg_color));
                            }
                        }
                    }
                    else
                    {
                        value = item.getValue();
                        this.data.put(ersKey, new DataItem(ersKey, item.getErsname(), value , item.getBgcolor()));
                    }
            
                }

		if (isInitialized) {
			log.debug("data initialized");
			synchronized(this) {
				this.notifyAll();
			}
		}
            }
            catch(Exception e) 
            {
                    try 
                    {
                            log.error("object update task error: "+e);
                            if (log.isDebugEnabled())
                                    e.printStackTrace();
                    }
                    catch(Exception e2) 
                    {
                            System.out.println("ERROR: object update task error: "+e);
                            //e.printStackTrace();
                    }
            }
            
	}

	@Override
	protected void finalize() throws Throwable {
		try {
			data=null;
		} finally {
			super.finalize();
		}
	}
}
