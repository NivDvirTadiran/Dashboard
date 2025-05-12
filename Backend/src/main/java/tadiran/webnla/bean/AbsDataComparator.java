/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package tadiran.webnla.bean;

import java.util.Comparator;


/**
 * compare data values of two NLA objects
 * @author evgeniv
 */
public class AbsDataComparator implements Comparator {
	private String itemKey;

	public AbsDataComparator(String key) {
		this.itemKey=key;
	}

	@Override
	public int compare (Object o1, Object o2) {
		Object v1=((AbsDataClass)o1).getDataValue(itemKey);
		Object v2=((AbsDataClass)o2).getDataValue(itemKey);
		if (v1==null)
			return -1;
		else
		if (v2==null)
			return -1;
                else{
                    String s1 = v1.toString();  
                    String s2 = v2.toString();
                     if (isNumeric(s1) && isNumeric(s2))
                     { 
                        int num1 = Integer.parseInt(s1);
                        int num2 = Integer.parseInt(s2);
                        return ((Comparable)num1).compareTo(num2);
                    }
                    else return ((Comparable)v1).compareTo(v2);
                }
	}
        
        public boolean isNumeric(String str) {
            return str.matches("-?\\d+(\\.\\d+)?");  //match a number with optional '-' and decimal.
          }

}
