package tadiran.webnla.bean;

import java.util.List;



/**
 * NLA IVR object
 * @author evgeniv
 */
public class IVRPort extends AbsDataClass {
	public IVRPort() {
		super(-1);
	}

	public IVRPort(int id) {
		super(id);
		setData("6_3_12_1_0", "IVRPort ID", id);
	}

	@Override
	public String getName() {
		return (String)getDataValue("6_3_12_1_1");
	}

	@Override
	public String getNum() {
		return (String)getDataValue("6_3_12_1_1");
	}

	@Override
	public void updateData(List items) {
		log.trace("update IVR Port data");
		super.updateStrData(items);

		this.id=Integer.parseInt((String)getDataValue("6_3_12_1_1"));
	}
}
