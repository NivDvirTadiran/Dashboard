package tadiran.webnla.bean;

import java.util.List;



/**
 * NLA IVR object
 * @author evgeniv
 */
public class IVRApp extends AbsDataClass {
	public IVRApp(int id) {
		super(id);
		setData("6_3_11_1_0", "IVRApp ID", id);
	}

	@Override
	public String getName() {
		return (String)getDataValue("6_3_11_1_1");
	}

	@Override
	public String getNum() {
		return getDataValue("6_3_11_1_0").toString();
	}

	@Override
	public void updateData(List items) {
		log.trace("update IVR App data");
		super.updateStrData(items);
	}
}
