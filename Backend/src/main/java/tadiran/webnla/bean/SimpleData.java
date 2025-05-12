package tadiran.webnla.bean;

import java.io.UnsupportedEncodingException;
import java.util.List;



/**
 * NLA IVR object
 * @author evgeniv
 */
public class SimpleData extends AbsDataClass {
	public SimpleData() {
		super(-1);
	}
    
    public SimpleData(int id) {
		super(id);
	}
    
    @Override
    public void setData(String itemKey, String itemName, Object itemValue) {
        if (itemValue instanceof String) {
            try {
                //6-Jun-2019 YR BZ#49945
                itemValue=new String(((String)itemValue).getBytes("ISO-8859-1"), "UTF-8");
            }
            catch (UnsupportedEncodingException ex) {
            }
        }
        super.setData(itemKey, itemName, itemValue);
    }
    
    @Override
	public String getName() {
        return null;
    }
    
    @Override
	public String getNum() {
		return null;
	}

	@Override
	public void updateData(List items) {
		log.trace("update data");
		super.updateStrData(items);
	}
}
