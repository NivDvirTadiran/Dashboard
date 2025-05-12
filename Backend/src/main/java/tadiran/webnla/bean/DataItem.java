package tadiran.webnla.bean;

import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;
import tadiran.webnla.tag.LocaleMessage;

/**
 * NLA object data
 * @author evgeniv
 */
public class DataItem{
    private static final Logger log = LogManager.getLogger("DataItem");
    protected String itemKey;
    protected String name;
    protected Object value;
    protected Object value2;

    /**
     * create data object
     * @param String - data name
     * @param String - data value
     */
    public DataItem(String itemKey, String name, Object value) {
        this.itemKey=itemKey;
        this.value=value;
        setName(name);
    }

    /**
     * create data object with 2 values
     * @param String - data name
     * @param String - data value1
     * @param String - data value2
     */
    public DataItem(String itemKey, String name, Object value, Object value2) {
        log.debug("New DataItem <"+itemKey+","+name+","+value+">");
        String valOut = null;
        //11-Jun-2019 YR BZ#50001
        //20-Jun-2019 YR BZ#50094
        if(name.contains("Name") || name.contains("Code") || name.contains("Group"))
        {
            try {
                valOut = new String(value.toString().getBytes("ISO-8859-1"), "UTF-8"); //convertFromUTF8
                this.value = valOut;
            } catch (java.io.UnsupportedEncodingException e) {
                //log.error("GroupsUpdater.forceUpdate - UnsupportedEncodingException: " + e);
            }
        }
        //16-Jun-2019 YR BZ#50047
        //else if(name.equals("State"))
        //{
        //    valOut = LocaleMessage.getMessage(value.toString(), "");
        //    this.value = valOut;
        //}
        else
            this.value = value;
                
        this.itemKey = itemKey;
        this.value2 = value2;
        setName(name);
        log.debug("New DataItem - OK");
    }

    public String getTranslatedValue(String langName) {
        return LocaleMessage.getMessage(value.toString(), langName);
    }
    public String getKey() {
        return this.itemKey;
    }
    
    public String getName() {
        return this.name;
    }
    
    public String getName(String langName) {
        String Name=LocaleMessage.getMessage(this.itemKey, langName);
        log.debug("getName for DataItem " + itemKey + ":" + name + " (" + langName + ") translate: " + Name);
        if (Name.isEmpty())
            return name;
        return Name;
    }
        
    public void setName(String name) {
        String lname=LocaleMessage.getMessage(this.itemKey, "en");
        if (lname.equals(this.itemKey))
            lname=name;
        this.name=lname;
    }

    public Object getValue() {
        return this.value;
    }

    public Object getBValue() {
        return this.value2;
    }

    /**
     * check data value is number
     * @return
     */
    public boolean getIsNumber() {
        try {
            if (this.value instanceof String)
                Long.parseLong((String) this.value, 10);
			
            return true;
        }
        catch(NumberFormatException e) {
            return false;
        }
    }

    /**
     * data to string
     * @return
     */
    @Override
    public String toString () {
        log.debug("toString <"+itemKey+","+name+","+value+">");
        //15-Aug-2019 YR BZ#50536
        if (this.name.indexOf("ime")>0 || 
            this.itemKey.equals("20_3_1_2_3") ||   //Group Average Q Time 
            this.itemKey.equals("20_3_1_2_4") ||   //Group Max Q Time 
            this.itemKey.equals("6_3_1_2_10") ||   //Group Max Q Time of outbound calls
            this.itemKey.equals("6_3_1_2_13") ||   //Group Average Time of outbound pending calls
            this.itemKey.equals("6_3_4_2_3") ||    //SG Average Q Time
            this.itemKey.equals("6_3_4_2_4") ||    //SG Max Q Time
            this.itemKey.equals("6_3_4_2_6") ||    //SG Average Wait Time of Outbound Call (pending)
            this.itemKey.equals("6_3_4_2_7") )     //SG Max Wait Time of Outbound Calls (pending)
        {
            log.debug("toString <"+itemKey+","+name+","+value+"> - Time Value");
            long value;
            if (this.value instanceof Long)
                value=(Long)this.value;
            else {
                try {
                    value=Long.parseLong((String)this.value);
                }
                catch(NumberFormatException e) {
                    return (String)this.value;
                }
            }

            //return String.format("%1$d:%2$02d",value/60,value%60);
            String frmOut;
            if (value/60<60)
                frmOut=String.format("%1$d:%2$02d",value/60,value%60);
            else
                frmOut=String.format("%1$d:%2$02d:%3$02d",value/3600,(value%3600)/60,value%60);

            log.debug("toString - OK");
            return frmOut;

        }
        else
            if (this.name.indexOf("%")!=-1)
            {
                log.debug("toString - OK");
                return this.value.toString()+"%";
            }
            else
            {
                log.debug("toString - OK");
                return this.value.toString().replace("<","&lt;").replace(">","&gt;");
            }
    }
}
