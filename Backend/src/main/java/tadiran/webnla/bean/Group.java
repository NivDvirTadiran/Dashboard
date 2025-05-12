package tadiran.webnla.bean;

import java.io.UnsupportedEncodingException;
import tadiran.emisweb.DataItemRow;
import tadiran.emisweb.DataItemType;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;



/**
 * NLA group object
 * @author evgeniv
 */
public class Group extends AbsDataClass {
	protected HashMap<Integer,Agent> agents;
	protected long agentsLastAccessTime;

	public Group() {
		super(-1);
		setData("6_3_1_2_5", " ", 0);
		setData("6_3_1_2_5g", " ", 0);
		setData("6_3_1_2_23", " ", 0);
		setData("6_3_1_2_23g", " ", 0);
                setData("20_3_1_2_5", " ", 0);
		setData("20_3_1_2_5g", " ", 0);
		agents=new HashMap(25, 0.5f);
	}

	public Group(int id) {
		super(id);
		setData("6_3_1_1_0", "GroupID", id);
		setData("6_3_1_2_5", " ", 0);
		setData("6_3_1_2_5g", " ", 0);
		setData("6_3_1_2_23", " ", 0);
		setData("6_3_1_2_23g", " ", 0);
                setData("20_3_1_2_5", " ", 0);
		setData("20_3_1_2_5g", " ", 0);
		agents=new HashMap(25, 0.5f);
	}

	public HashMap<Integer,Agent> getAgentsData() {
		lastAccessTime=System.currentTimeMillis();
		agentsLastAccessTime=lastAccessTime;
		return this.agents;
	}

	public boolean isAgentsExpired() {
		return (System.currentTimeMillis()-agentsLastAccessTime)>AbsDataClass.ACCESS_TIMEOUT;
	}

	@Override
	public String getName() {
            try {
		log.debug("Group:getName - returning getDataValue(\"6_3_1_1_1\").toString()" + getDataValue("6_3_1_1_1").toString() + "(encoded " + getDataValue("6_3_1_1_1").toString().getBytes("ISO-8859-1") + ")");
            }
            catch (UnsupportedEncodingException ex) {
            }
		return (String)getDataValue("6_3_1_1_1");
	}

	@Override
	public String getNum() {
		return getDataValue("6_3_1_1_2").toString();
	}

	public void updateAgentsData(List<DataItemRow> items) {
		log.trace("update group's agents data");
		Iterator<DataItemRow> iterator = items.iterator();
		List<DataItemType> agentData;
		DataItemRow agentRow;
		Agent agent;
		agents.clear();
		while (iterator.hasNext()) {
			agent=new Agent();
			agentRow= iterator.next();
			agentData= (List<DataItemType>) agentRow.getReturnArray();

			agent.updateData(agentData);
			agents.put(agent.id, agent);
		}
	}
}
