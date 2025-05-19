package tadiran.webnla.synch;

import org.springframework.ws.WebServiceException;
import tadiran.emisweb.EMISwebPort;
import tadiran.emisweb.RequestInfoHeaderType;
import tadiran.webnla.AbsCollection;
import tadiran.webnla.NLAContext;
import tadiran.webnla.bean.SessionData;
import tadiran.webnla.endpoint.EMISwebServiceClient;
import tadiran.webnla.servlet.NLAService;
import java.net.MalformedURLException;
import java.util.HashMap;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;


/**
 * abstract NLA object updater task
 * @author evgeniv
 */
public abstract class AbsUpdater extends TimerTask {
	protected static final Logger log;
	protected static final int INTERVAL=1000;	// data request interval after timeout
	protected static Timer updateTimer=new Timer("AbsUpdater");
	protected static NLAContext ctx=NLAContext.getInstance();
	protected static EMISwebPort wsEMIS_port;
	private   static int updater_connected_server = NLAService.NOT_CONNECTED;

	protected RequestInfoHeaderType reqHeader;
	protected AbsCollection collection;
	protected HashMap collectionData;
    
    static {
        log = LogManager.getLogger("AbsUpdater");
        try {
            switch (NLAService.getConnectedServer()) {
                case NLAService.PRIMARY_SERVER:
                    wsEMIS_port = new EMISwebServiceClient(NLAContext.ACCServerAddress1,NLAContext.ACCServerPort1).getEMISwebPort();
                    updater_connected_server = NLAService.PRIMARY_SERVER;
                    break;
                case NLAService.SECONDARY_SERVER:
                    wsEMIS_port = new EMISwebServiceClient(NLAContext.ACCServerAddress2,NLAContext.ACCServerPort2).getEMISwebPort();
                    updater_connected_server = NLAService.SECONDARY_SERVER;
                    break;
                default:
                    log.warn("WARNING, NLAService NOT Connected");
                    break;
            }
        }
        catch (MalformedURLException ex) {
            log.fatal("fatal error, cannot access web services: "+ex.getMessage());
        }
    }


    protected AbsUpdater(AbsCollection p_collection) {	
        if (wsEMIS_port!=null) {
            log.debug("CREATE nla object updater task");
            collection=p_collection;
            collectionData=collection.getCollection();

            reqHeader=new RequestInfoHeaderType();
            reqHeader.setServiceReqId(1);

            updateTimer.schedule(this, 0, getUpdateInterval());
        }
        else
            log.fatal("fatal error, cannot access web services");
    }
    
    protected int getUpdateInterval() {
        return this.INTERVAL;
    }

    //10-Aug-2022 YR BZ#56641
    protected void updateServerTime(String server_time)
    {
        List<SessionData> sessions = ctx.getSessions();
        for (int i=0; i<sessions.size(); i++)
        {
            //log.info("Session(" + i + ":" + sessions.get(i).getHttpSession() + ") updateServerTime - " + server_time);
            //sessions.get(i).getHttpSession().setAttribute("accServerTime", server_time);
        }
    }
    
    public void forceUpdate() {
        try {
            onUpdate();
        }
        catch (Exception ex) {
        }
    }
	
    public void reconnect() {
        log.info("*reconnect");
        try {
            updater_connected_server = NLAService.NOT_CONNECTED;
            if( NLAService.getConnectedServer() != NLAService.NOT_CONNECTED )
            {
                log.info("NLAUpdater: no connection or connected to wrong server, reconnect..");
                log.info("NLAUpdater: call NLAService.register(true)");
                NLAService.register(true);
                log.info("NLAUpdater: call NLAService.createConnection(NLAService.getConnectedServer()");
                wsEMIS_port = NLAService.createConnection(NLAService.getConnectedServer());
                log.info("NLAUpdater: OK");
                updater_connected_server = NLAService.getConnectedServer();

                List<SessionData> sessions = ctx.getSessions();
                for (int i=0; i<sessions.size(); i++)
                    sessions.get(i).reconnect();
            }
        }
        catch (MalformedURLException ex) {
            log.fatal("fatal error MalformedURLException - cannot access web services: "+ex.getMessage());
        }
        catch (Exception ex1) {
            log.fatal("fatal error Exception - cannot access web services: "+ex1.getMessage());
        }
    }
	
	protected abstract void onUpdate() throws Exception;

	@Override
	public void run (){
            boolean reconnect = false;
            if (collection.isExpired())
                return;
		
            log.debug("AbsUpdater on timer");
            try {
                if (wsEMIS_port==null)
                {
                    log.info("NLAUpdater: wsEMIS_port is null");
                    reconnect = true;
                }
                if (updater_connected_server != NLAService.getConnectedServer())
                {
                    log.info("NLAUpdater: updater_connected_server(" + updater_connected_server + ") is diff from NLAService.getConnectedServer(" + NLAService.getConnectedServer() + ")");
                    reconnect = true;
                }
                
                if (reconnect)
                {
                    wsEMIS_port=null;
                    updater_connected_server = NLAService.NOT_CONNECTED;
                    if( NLAService.getConnectedServer() != NLAService.NOT_CONNECTED )
                    {
                        log.info("NLAUpdater: no connection or connected to wrong server, reconnect..");
                        //log.info("NLAUpdater: call NLAService.register(true)");
                        //NLAService.register(reconnect);
                        log.info("NLAUpdater: call NLAService.createConnection(NLAService.getConnectedServer()");
                        wsEMIS_port = NLAService.createConnection(NLAService.getConnectedServer());
                        log.info("NLAUpdater: OK");
                        updater_connected_server = NLAService.getConnectedServer();

                        List<SessionData> sessions = ctx.getSessions();
                        for (int i=0; i<sessions.size(); i++)
                            sessions.get(i).reconnect();
                    }
                }

                reqHeader.setTicket(ctx.getValidTicket());
                if (wsEMIS_port!=null)
                    onUpdate();

                if (!collection.isInitialized())
                    synchronized(collectionData) {
                        collectionData.notifyAll();
                        log.trace("initial data updated");
                    }
            }
            catch(WebServiceException e) {
                log.warn("NLAUpdater Cought WebServiceException - " + e.getMessage());
                //wsEMIS_port = null;
            }
            catch(Exception e) {
                log.warn("NLAUpdater Cought Exception - " + e.getMessage());
                try {
                    log.error("object update task error: "+e);
                    //if (log.isDebugEnabled())
                        e.printStackTrace();
                }
                catch(Exception e2) {
                    System.out.println("ERROR: object update task error: "+e);
                    //e.printStackTrace();
                }
            }
	}

	@Override
	protected void finalize() throws Throwable {
		log.debug("*** DESTROY nla object updater task");
		cancel();
		super.finalize();
	}
}
