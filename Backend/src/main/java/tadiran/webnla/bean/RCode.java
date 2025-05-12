/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tadiran.webnla.bean;

import java.util.List;

/**
 *
 * @author a180042
 */
public class RCode extends AbsDataClass {
    
	public RCode(int id) {
		super(id);
		setData("6_3_2_2_11", "RCode ID", id);
	}

	@Override
	public String getName() {
		return (String)getDataValue("6_3_2_2_11");
	}

	@Override
	public String getNum() {
		return getDataValue("6_3_2_2_11").toString();
	}

	@Override
	public void updateData(List items) {
		log.trace("update RCode data");
		super.updateStrData(items);
	}
}
