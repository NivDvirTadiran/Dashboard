package tadiran.webnla.bean;

import java.util.List;



/**
 * NLA agent object
 * @author evgeniv
 */
public class Agent extends AbsDataClass {
	public Agent() {
		super(-1);
                setData("6_3_1_1_2", "ACD Group", "");
	}

	public Agent(int id) {
		super(id);
		setData("6_3_2_1_0", "AgentID", id);
	}

	@Override
	public String getName() {
		return (String)getDataValue("6_3_2_1_1");
	}

	@Override
	public String getNum() {
		return getDataValue("6_3_2_1_2").toString();
	}

	public String getExt() {
		return (String)getDataValue("6_3_2_1_3");
	}

	@Override
	public void updateData(List items) {
		log.trace("update agent data");
		super.updateStrData(items);

		this.id=Integer.parseInt((String)getDataValue("6_3_2_1_0"));
	}
}
