package tadiran.webnla.bean;

import tadiran.emisweb.DataItemRow;
import tadiran.emisweb.DataItemType;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;



/**
 * NLA group object
 * @author evgeniv
 */
public class SuperGroup extends AbsDataClass {
	protected HashMap<Integer,Group> groups;
	protected long groupsLastAccessTime;

	public SuperGroup() {
		super(-1);
		groups=new HashMap(25, 0.5f);
	}

	public SuperGroup(int id) {
		super(id);
		groups=new HashMap(25, 0.5f);
	}

	public HashMap<Integer,Group> getGroupsData() {
		lastAccessTime=System.currentTimeMillis();
		groupsLastAccessTime=lastAccessTime;
		return this.groups;
	}

	public boolean isAgentsExpired() {
		return (System.currentTimeMillis()-groupsLastAccessTime)>AbsDataClass.ACCESS_TIMEOUT;
	}

	@Override
	public String getName() {
		return (String)getDataValue("6_3_4_1_1");
	}

	@Override
	public String getNum() {
            return null;
//		return getDataValue("6_3_1_1_2").toString();
	}

	public void updateGroupsData(List<DataItemRow> items) {
		log.trace("update SuperGroup's groups data");
		Iterator<DataItemRow> iterator = items.iterator();
		List<DataItemType> groupData;
		DataItemRow groupRow;
		Group group;
		groups.clear();
		while (iterator.hasNext()) {
			group=new Group();
			groupRow= iterator.next();
			groupData= (List<DataItemType>) groupRow.getReturnArray();

			group.updateData(groupData);
			groups.put(group.id, group);
		}
	}
}
