package tadiran.webnla;

import tadiran.webnla.accdashboard.services.ChartService;
import tadiran.webnla.bean.AbsDataClass;
import tadiran.webnla.bean.Group;
import tadiran.webnla.bean.IVRGroup;
import tadiran.webnla.bean.PeriodData;
import tadiran.webnla.bean.SessionData;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;
import jakarta.servlet.http.HttpSession;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;


/**
 * NLA context
 * @author evgeniv
 */
public class NLAContext
{
    private static final Logger log = LogManager.getLogger(ChartService.class);
    public static String ACCServerAddress1;
    public static String ACCServerPort1;
    public static String ACCServerAddress2;
    public static String ACCServerPort2;
    public static String ACCWSList;
    public static String ACCVersion;
    public static int MaxAgentsInBriefReport;
    private static List<SessionData> sessions;
    private static NLAContext inst;
    private static Groups groups;
    private static Agents agents;
    private static DNISs dniss;
    private static RCodes rCodes;
    private static IVRApps ivrApps;
    private static IVRGroups ivrGrps;
    private static SuperGroups superGroups;
    private static GroupPeriods g_periods;
    private static SuperGroupPeriods sg_periods;


    public static NLAContext getInstance()
    {
        log.trace("get instance of NLAcontext");
        if (inst==null)
        {
            inst=new NLAContext();
            // create objects repository
            groups=new Groups();
            agents=new Agents();
            dniss=new DNISs();
            ivrApps=new IVRApps();
            rCodes=new RCodes();
            ivrGrps=new IVRGroups();
            superGroups=new SuperGroups();
            g_periods=new GroupPeriods();
            sg_periods=new SuperGroupPeriods();
            log.info("NLA context initialize");
        }

        return inst;
    }

    /**
     * get WebNLA logged sessions
     * @return
     */
    public List<SessionData> getSessions()
    {
        if (sessions==null)
            sessions=new Vector<SessionData>();

        return sessions;
    }

    public String getValidTicket()
    {
        if (sessions!=null && !sessions.isEmpty())
            return sessions.get(0).getTicket();

        return null;
    }

    public boolean isSessionExist(HttpSession session)
    {
        if (sessions != null && !sessions.isEmpty())
        {
            for (int i=0; i<sessions.size(); i++)
            {
                SessionData sd = sessions.get(i);
                if(sd.getHttpSession() == session)
                {
                    return true;
                }
            }
        }
        return false;
    }

    
//    public void invalidateOldUserSession(LoginRequestType loginData)
//    {
//        if (sessions!=null && !sessions.isEmpty())
//        {
//            for (int i=0; i<sessions.size(); i++)
//            {
//                SessionData sd = sessions.get(i);
//                if( loginData.getName().equals(sd.getLoginData().getName()) &&
//                    loginData.getPass().equals(sd.getLoginData().getPass()) &&
//                    loginData.getServiceReqId() == (sd.getLoginData().getServiceReqId()) &&
//                    loginData.getApplicationId().equals(sd.getLoginData().getApplicationId()) )
//                {
//                    log.info("User " + loginData.getName() + " is already exist, invalidate old session");
//                    sd.logout(true);
//                    sd.getHttpSession().invalidate();
//                    sd = null;
//                    //return true;
//                }
//            }
//        }
//        //return false;
//    }

    /**
     * get collection of active groups
     * @return Groups
     */
    public Groups getGroups() {
        return groups;
    }

    public GroupPeriods getGroupPeriods() {
        return g_periods;
    }

    public SuperGroupPeriods getSuperGroupPeriods() {
        return sg_periods;
    }

    /**
     * get collection of active agents
     * @return Agents
     */
    public Agents getAgents() {
        return agents;
    }

    public DNISs getDNISs() {
        return dniss;
    }

    public RCodes getRCodes() {
        return rCodes;
    }

    public IVRApps getIVRApps() {
        return ivrApps;
    }

    public IVRGroups getIVRGroups() {
            return ivrGrps;
    }

    public SuperGroups getSuperGroups() {
            return superGroups;
    }

    /**
     * get multiple active NLA objects by name
     * @param ref - objects name, for example: Groups or Group:1,Group:2
     * @return Map
     */
    public Map<Integer,? extends AbsDataClass> getObjectMap(String ref) throws InterruptedException {
        if (ref==null)
            return null;

        if (ref.equals("Groups"))
            return groups.getData();

        else if (ref.equals("Agents"))
            return agents.getData();

        else if (ref.equals("DNISs"))
            return dniss.getData();

        else if (ref.equals("RCodes"))
            return rCodes.getData();

        else if (ref.equals("IVRApps"))
            return ivrApps.getData();

        else if (ref.equals("IVRGroups"))
            return ivrGrps.getData();

        else if (ref.equals("SuperGroups"))
            return superGroups.getData();

        else if (ref.startsWith("Group") && ref.endsWith(".Agents"))
        {
            Group group=(Group)getObject(ref.substring(0,ref.indexOf(".Agents")));
            //if (group!=null)
            return group.getAgentsData();
            //else
            //	return new HashMap();
        }
            
        else if (ref.startsWith("IVRGroup") && ref.endsWith(".Ports"))
        {
            IVRGroup ivrGroup=(IVRGroup)getObject(ref.substring(0,ref.indexOf(".Ports")));
            return ivrGroup.getPortsData();
        }

        else if ((ref.startsWith("Group") || ref.startsWith("SuperGroup")) && ref.endsWith(".Periods"))
        {
            PeriodData period=(PeriodData)getObject(ref.substring(0,ref.length()-1));
            return period.getHourlyData();
        }
        else
        {
            Map<Integer,AbsDataClass> resp=null;
            String []refs=ref.split(",");
            resp = new HashMap(refs.length);
            AbsDataClass obj;
            for(int i=0; i<refs.length; i++)
            {
                obj=getObject(refs[i]);
                if (obj!=null)
                    resp.put(obj.getId(), obj);
            }
            return resp;
        }
    }

    /**
     * get single NLA object by name
     * @param name - object name, for example: Group:1
     * @return AbsDataClass
     */
    public AbsDataClass getObject(String name) throws InterruptedException {

        if (name==null || name.indexOf(':')==-1)
            return null;


        String[] names=name.split("\\.");
        String[] objRef=names[0].split(":");
        HashMap<Integer,? extends AbsDataClass> objsMap=null;
        if (objRef[0].equals("Group"))
        {
            if (names.length==1)
                objsMap=groups.getData();
            else if (names[1].startsWith("Period"))
                objsMap=g_periods.getData();
        }
        else if (objRef[0].equals("Agent"))
            objsMap=agents.getData();
        else if (objRef[0].equals("DNIS"))
            objsMap=dniss.getData();
        else if (objRef[0].equals("RCodes"))
            objsMap=rCodes.getData();
        else if (objRef[0].equals("IVRApplication"))
            objsMap=ivrApps.getData();
        else if (objRef[0].equals("IVRGroup"))
            objsMap=ivrGrps.getData();
        else if (objRef[0].equals("SuperGroup"))
        {
            if (names.length==1)
                objsMap=superGroups.getData();
            else if (names[1].startsWith("Period"))
                objsMap=sg_periods.getData();
        }

        if (objsMap!=null) {
            return objsMap.get(Integer.parseInt(objRef[1]));
        }
        else
            return null;
    }
	
    public static void destroy()
    {
        if (inst==null)
            return;

        log.info("******** DESTROY NLA context");
        
        for (int i=0; i<sessions.size(); i++)
        {
            SessionData sd = sessions.get(i);
            //log.info("User " + loginData.getName() + " is already exist, invalidate old session");
            sd.logout(true);
            //sd.getHttpSession().invalidate();
            sd = null;
            //return true;
        }
        
        groups=null;
        agents=null;
        dniss=null;
        superGroups=null;
    }
}
