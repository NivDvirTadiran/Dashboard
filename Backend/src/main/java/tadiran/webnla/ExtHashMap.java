package tadiran.webnla;

import java.util.HashMap;


/**
 * Hashmap with access notification
 * @author evgeniv
 */
public class ExtHashMap<KeyType,DataType> extends HashMap<KeyType,DataType> {
	private ExtHashMapNotification handler;

	public void setBeforeGet(ExtHashMapNotification phandler) {
		handler=phandler;
	}

	@Override
	public DataType get(Object pkey) {
		if (pkey==null)
			return null;

		Integer key=null;
		if (pkey instanceof String)
			key=Integer.parseInt((String)pkey);
		else
		if (pkey instanceof Long)
			key=((Long)pkey).intValue();
		else
			key=(Integer)pkey;

		if (!super.containsKey(key))
			handler.beforeGet(key); //notify listener
		
		return super.get(key);
	}
}
