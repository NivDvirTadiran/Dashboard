package tadiran.webnla.bean;

import tadiran.emisweb.DataItemType;
import java.util.HashMap;
import java.util.List;



/**
 * NLA IVR object
 * @author evgeniv
 */
public class IVRGroup extends AbsDataClass {
	protected HashMap<Integer,IVRPort> ports;

	public IVRGroup(int id) {
		super(id);
		setData("6_3_13_1_0", "IVRGrp ID", id);
		ports=new HashMap(10, 0.5f);
	}

	@Override
	public String getName() {
		return (String)getDataValue("6_3_13_1_1");
	}

	@Override
	public String getNum() {
		return getDataValue("6_3_13_1_0").toString();
	}

	@Override
	public void updateData(List items) {
		log.trace("update IVR Group data");
		super.updateStrData(items);

		ports.clear();
	}

	public HashMap<Integer,IVRPort> getPortsData() {
		return this.ports;
	}

	public void addPortsData(List<DataItemType> items) {
		log.trace("update group's agents data");
		IVRPort port=new IVRPort();
		port.updateData(items);
		ports.put(port.id, port);
	}
}
