package tadiran.webnla.bean;

import tadiran.emisweb.DataItemRow;

import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;



/**
 * NLA IVR object
 * @author evgeniv
 */
public class PeriodData extends SimpleData {
    //public static final int ACCESS_TIMEOUT=1000*60*1; // 1min
    protected LinkedHashMap<Integer,SimpleData> hourlyData;
    
    public PeriodData(int id) {
        super(id);
        hourlyData=new LinkedHashMap<Integer, SimpleData>();
    }
    
    public LinkedHashMap<Integer,SimpleData> getHourlyData() {
        lastAccessTime=System.currentTimeMillis();
        return hourlyData;
    }
    
    public void updateHourlyData(List<DataItemRow> rowsHourlyData) {
        LinkedHashMap<Integer,SimpleData> l_hourlyData=new LinkedHashMap<Integer, SimpleData>();
        int i=0;
        for(DataItemRow rowData: rowsHourlyData) {
            SimpleData data=new SimpleData();
            data.updateData(Collections.singletonList(rowData.getReturnArray()));
            l_hourlyData.put(i++, data);
        }
        hourlyData=l_hourlyData;
    }
}
