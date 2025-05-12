package tadiran.webnla.bean;

import java.util.List;



/**
 * NLA DNIS object
 * @author evgeniv
 */
public class DNIS extends AbsDataClass {
	public DNIS() {
		super(-1);
	}

	@Override
	public String getName() {
		return (String)getDataValue("6_3_10_1_1");
	}

	@Override
	public String getNum() {
		return getDataValue("6_3_10_1_2").toString();
	}

	@Override
	public void updateData(List items) {
		log.trace("update DNIS data");
		super.updateStrData(items);

		this.id=Integer.parseInt((String)getDataValue("6_3_10_1_0"));
	}
}
