package tadiran.webnla.servlet;

import org.springframework.core.io.ClassPathResource;
import tadiran.emisweb.ApplicationKeepAliveRequestType;
import tadiran.emisweb.ApplicationRegisterRequestType;
import tadiran.emisweb.ApplicationReturnType;
import tadiran.emisweb.EMISwebPort;
import tadiran.webnla.NLAContext;
import tadiran.webnla.config.PropertiesManager;
import tadiran.webnla.endpoint.EMISwebServiceClient;
import tadiran.webnla.tag.LocaleMessage;
import java.io.*;
import java.util.*;
import jakarta.servlet.ServletConfig;
import jakarta.servlet.ServletContext;
import jakarta.servlet.ServletException;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;

/**
 * NLA loader class, initiated when webNLA application started at the first time
 * @author evgeniv
 */
public class NLAService extends HttpServlet {
	private static final Logger log = LogManager.getLogger("NLAService");
	private static EMISwebPort wsEMIS_port_P;
	private static EMISwebPort wsEMIS_port_S;
	private static final int KEEPALIVE_TIMEOUT=1000*10; //30sec
	public  static final int NOT_CONNECTED=0;
	public  static final int PRIMARY_SERVER=1;
	public  static final int SECONDARY_SERVER=2;
	private static final Timer timer=new Timer("NLAService");
	private static String appID;
	private static int connectedServer;
	private static ApplicationKeepAliveRequestType keepAliveReq;
	private static ApplicationRegisterRequestType appRegParams;
        public  static boolean SSO_flag;
        public  static boolean DP_APP_CENTER;

	@Override
	public void init (ServletConfig config) throws ServletException {
        try {
            super.init(config);
            log.info("++++++++++  init  +++++++++");

            ServletContext ctx=config.getServletContext();

            String ACCServerAddress1 = ((String) PropertiesManager.getProperty("tadiran.dashboard.ACCServerAddress1", String.class));
            log.info("NLAService, ACCServerAddress1: " + ACCServerAddress1);

            String ACCServerAddress2 = ((String) PropertiesManager.getProperty("tadiran.dashboard.ACCServerAddress2", String.class));
            log.info("NLAService, ACCServerAddress2: " + ACCServerAddress2);

            String ACCServerPort1 = ((String) PropertiesManager.getProperty("tadiran.dashboard.ACCServerPort1", String.class));
            log.info("NLAService, ACCServerPort1: " + ACCServerPort1);

            String ACCServerPort2 = ((String) PropertiesManager.getProperty("tadiran.dashboard.ACCServerPort2", String.class));
            log.info("NLAService, ACCServerPort2: " + ACCServerPort2);

            String appID = ((String) PropertiesManager.getProperty("tadiran.dashboard.appID", String.class));
            log.info("NLAService, appID: " + appID);

            String ACCWebServers = ((String) PropertiesManager.getProperty("tadiran.dashboard.ACCWEBServers", String.class));
            log.info("NLAService, ACCWEBServers: " + ACCWebServers);

            String ACCVersion = ((String) PropertiesManager.getProperty("tadiran.dashboard.ACCVersion", String.class));
            log.info("NLAService, ACCWEBServers: " + ACCVersion);

            int MaxAgentsInBriefReport = 200;//Integer.parseInt(ctx.getInitParameter("MaxAgentsInBriefReport"));
            NLAContext.ACCServerAddress1 = ACCServerAddress1;
            NLAContext.ACCServerAddress2 = ACCServerAddress2;
            NLAContext.ACCServerPort1 = ACCServerPort1;
            NLAContext.ACCServerPort2 = ACCServerPort2;
            NLAContext.ACCWSList = ACCWebServers;
            NLAContext.ACCVersion = ACCVersion;
            NLAContext.MaxAgentsInBriefReport = MaxAgentsInBriefReport;
            connectedServer = NOT_CONNECTED;
            ctx.setAttribute("NLAcontext", NLAContext.getInstance());
            load(ctx);
            SSO_flag = false;

            DP_APP_CENTER = ((Boolean) PropertiesManager.getProperty("tadiran.dashboard.IsAuthTokenFilterOn", Boolean.class));

            appRegParams = new ApplicationRegisterRequestType();
            appRegParams.setServiceReqId(1);
            appRegParams.setApplicationId(appID);
            keepAliveReq = new ApplicationKeepAliveRequestType();
            keepAliveReq.setApplicationId(appID);

            try {
                        if (!register(true))
                            throw new ServletException("Registration into WebNLA services failed");
            }
            catch(Exception ignored){}
        } catch (IOException e) {
            throw new RuntimeException(e);
        } finally {
		timer.schedule(new KeepAliveTask(),KEEPALIVE_TIMEOUT,KEEPALIVE_TIMEOUT);
        }
	}
/*
	@Override
	protected void finalize() throws Throwable {
        try {
            timer.cancel();
        } finally {
            super.finalize();
        }
	}
	*/
	public static boolean isConnected() {
		return keepAlive(false);
	}

	private static void keepAlive() {
		keepAlive(true);
	}

	private static boolean keepAlive(boolean isReconnect) {
		ApplicationReturnType app_keep_alive_resp = null;
		try {
                    if ((connectedServer == PRIMARY_SERVER) && 
                        (wsEMIS_port_P != null) )
                    {
                        log.debug("*KeepAlive - Connected to PRIMARY");
                        keepAliveReq.setServiceReqId(System.currentTimeMillis());
                        app_keep_alive_resp = wsEMIS_port_P.applicationKeepAlive(keepAliveReq);
                    }
                    else if((connectedServer == SECONDARY_SERVER) && 
                            (wsEMIS_port_S != null))
                    {
                        log.debug("*KeepAlive - Connected to SECONDARY");
                        keepAliveReq.setServiceReqId(System.currentTimeMillis());
                        app_keep_alive_resp = wsEMIS_port_S.applicationKeepAlive(keepAliveReq);
                    }
                    else if (isReconnect) 
                    {
                        log.warn("*KeepAlive - no connection.  call register(true)");
                        register(true);
                        return true;
                    }
                    
                    if(app_keep_alive_resp != null)
                        log.info("Servers Info: " + app_keep_alive_resp.getResponseInfoHeader().getServersInfo());
                    
                    if( (app_keep_alive_resp == null) ||
                        !connectToActive(app_keep_alive_resp.getResponseInfoHeader().getServersInfo()) )
                    {
                        log.info("*KeepAlive - Not connected to ACTIVE Server");
                        if (isReconnect) 
                        {
                            log.info("*KeepAlive - call register(true)");
                            register(true);
                            return true;
                        }
                    }
                    else
                    {
                        return true;
                    }
                
		}
		catch(Exception e) {
                    log.error("*KeepAlive connection problem: WebNLA service is unavailable"+e.getMessage());
                    if ((connectedServer == PRIMARY_SERVER) && 
                        (wsEMIS_port_P != null) )
                    {
                        System.out.println("WebNLA service is unavailable"+e.getMessage());
                        wsEMIS_port_P=null;
                    }
                    else if ((connectedServer == SECONDARY_SERVER) && 
                             (wsEMIS_port_S != null) )
                    {
                        System.out.println("WebNLA service is unavailable"+e.getMessage());
                        wsEMIS_port_S=null;
                    }
                    
                    if (isReconnect) 
                    {
                        try {
                            register(true);
                        }
                        catch(Exception e2) {
                            log.error("*Exception on register - "+e2.getMessage());
                        }
                        return true;
                    }
		}

		return false;
	}

	public synchronized static boolean register(boolean first_time) throws Exception {
		ApplicationReturnType appRegresp = null;
                
                if(first_time)
                {
                    log.info("*Registering application first time");
                    connectedServer = NOT_CONNECTED;
                    wsEMIS_port_P = null;
                    wsEMIS_port_S = null;
                }

                /* Create the connection*/
		try {
                    if ((wsEMIS_port_P == null) && (connectedServer == NOT_CONNECTED))
                    {
                        log.info("*connecting WebNLA to PRIMARY server..");
                        wsEMIS_port_P = createConnection(PRIMARY_SERVER);
                    }

 		}
		catch(Exception e) {
                    log.fatal("*Cannot connect to PRIMARY server: "+e.getMessage());
                    //throw e;
		}

                if( connectedServer == NOT_CONNECTED )
                {
                    try {
                        if (wsEMIS_port_S == null)
                        {
                            log.info("*connecting WebNLA to SECONDARY server..");
                            wsEMIS_port_S = createConnection(SECONDARY_SERVER);
                        }

                    }
                    catch(Exception e) {
                        log.fatal("*Cannot connect to SECONDARY server: "+e.getMessage());
                        //throw e;
                    }
                }
                
                
                /* Registering the application*/
                if( (connectedServer == PRIMARY_SERVER) &&
                    (wsEMIS_port_P != null) )
                {
                    log.info("*registering WebNLA to PRIMARY server..");
                    appRegresp = wsEMIS_port_P.applicationRegister(appRegParams);
                    if(appRegresp != null)
                        log.info("Servers Info: " + appRegresp.getResponseInfoHeader().getServersInfo());
                    
                    if ( (appRegresp == null) ||
                         (appRegresp.getResponseInfoHeader().getErrorCause()!=0) )
                    {
                        log.error("*Registration into WebNLA PRIMARY server failed");
                        wsEMIS_port_P = null;
                    }
                    else if((!connectToActive(appRegresp.getResponseInfoHeader().getServersInfo())))
                    {
                        log.info("*Connection is NOT to ACTIVE Server...  Reconnect");
                        wsEMIS_port_P = null;
                        try {
                            log.info("*connecting WebNLA to SECONDARY server..");
                            wsEMIS_port_S = createConnection(SECONDARY_SERVER);
                            log.info("*registering WebNLA to SECONDARY server..");
                            appRegresp = wsEMIS_port_S.applicationRegister(appRegParams);
                        }
                        catch(Exception e) {
                            log.fatal("*Cannot register to WebNLA SECONDARY services: "+e.getMessage());
                            throw e;
                        }
                    }
                }
                else if( (connectedServer == SECONDARY_SERVER) &&
                         (wsEMIS_port_S != null) )
                {
                    log.info("*registering WebNLA to SECONDARY server..");
                    appRegresp = wsEMIS_port_S.applicationRegister(appRegParams);
                    if(appRegresp != null)
                        log.info("Servers Info: " + appRegresp.getResponseInfoHeader().getServersInfo());
                    if ( (appRegresp == null) ||
                         (appRegresp.getResponseInfoHeader().getErrorCause()!=0) )
                    {
                        log.error("*Registration into WebNLA SECONDARY server failed");
                        wsEMIS_port_S = null;
                    }
                    else if((!connectToActive(appRegresp.getResponseInfoHeader().getServersInfo())))
                    {
                        log.info("*Connection is NOT to ACTIVE Server...  Reconnect");
                        wsEMIS_port_S = null;
                        try {
                            log.info("*connecting WebNLA to PRIMARY server..");
                            wsEMIS_port_P = createConnection(PRIMARY_SERVER);
                            log.info("*registering WebNLA to PRIMARY server..");
                            appRegresp = wsEMIS_port_P.applicationRegister(appRegParams);
                        }
                        catch(Exception e) {
                            log.fatal("*Cannot register to WebNLA SECONDARY services: "+e.getMessage());
                            throw e;
                        }
                    }
                }
                else
                {
                    log.info("*couldn't register WebNLA - UNKNOWEN case..");
                    return false;
                }
                
                
                /* Reconnect to other server*/
//		if ( (appRegresp != null) && 
//                     !connectToActive(appRegresp.getResponseInfoHeader().getServersInfo()) )
//                {
//                    log.info("*Connection is NOT to ACTIVE Server...  Reconnect");
//                    if(connectedServer == PRIMARY_SERVER)
//                    {
//                        wsEMIS_port_P = null;
//                        try {
//                            log.info("*connecting WebNLA to SECONDARY server..");
//                            wsEMIS_port_S = createConnection(SECONDARY_SERVER);
//                            log.info("*registering WebNLA to SECONDARY server..");
//                            appRegresp = wsEMIS_port_S.applicationRegister(appRegParams);
//                        }
//                        catch(Exception e) {
//                                log.fatal("*Cannot register to WebNLA SECONDARY services: "+e.getMessage());
//                                throw e;
//                        }
//                    }
//                    else if(connectedServer == SECONDARY_SERVER)
//                    {
//                        wsEMIS_port_S = null;
//                        try {
//                            log.info("*connecting WebNLA to PRIMARY server..");
//                            wsEMIS_port_P = createConnection(PRIMARY_SERVER);
//                            log.info("*registering WebNLA to PRIMARY server..");
//                            appRegresp = wsEMIS_port_P.applicationRegister(appRegParams);
//                        }
//                        catch(Exception e) {
//                                log.fatal("*Cannot register to WebNLA PRIMARY services: "+e.getMessage());
//                                throw e;
//                        }
//                    }
//                    
//                }
                
		if ( (appRegresp != null) && 
                      connectToActive(appRegresp.getResponseInfoHeader().getServersInfo()) )
                {
                    log.info("*Registration succeeded");
                    log.info("Servers Info: " + appRegresp.getResponseInfoHeader().getServersInfo());
                }
                
		return true;
	}

        
	public synchronized static EMISwebPort createConnection(int server) throws Exception {
            EMISwebPort conn_p,conn_s;
            log.info("*createConnection to " + server + "..");
            switch (server)
            {
                case PRIMARY_SERVER:
                    try {
                        log.info("*Connecting to WebNLA PRIMARY server on " + NLAContext.ACCServerAddress1 + "..");
                        conn_p = new EMISwebServiceClient(NLAContext.ACCServerAddress1,NLAContext.ACCServerPort1).getEMISwebPort();
                        connectedServer = PRIMARY_SERVER;
                    }
                    catch(Exception e) {
                        log.fatal("*Cannot connect to WebNLA services: " + e.getMessage());
                        throw e;
                    }
                    return conn_p;
                case SECONDARY_SERVER:
                    try {
                        log.info("*Connecting to WebNLA SECONDARY server on " + NLAContext.ACCServerAddress2 + "..");
                        conn_s = new EMISwebServiceClient(NLAContext.ACCServerAddress2,NLAContext.ACCServerPort2).getEMISwebPort();
                        connectedServer = SECONDARY_SERVER;
                    }
                    catch(Exception e) {
                        log.fatal("*Cannot connect to WebNLA services: " + e.getMessage());
                        throw e;
                    }
                    return conn_s;
                default:
                    log.error("*createConnection to UNKNOWN Connection");
                    break;
            }
            
            return null;
	}

	@Override
	public void destroy () {
                log.info("destroy() invoked");
                if (timer!=null)
                {
                    log.info("Stopping the keepalive timer thread...");
                    timer.cancel();
                    timer.purge();
                }
                NLAContext.destroy();
		super.destroy();
	}

    // load general NLA data, such as templates list, translation table
    public LinkedHashMap<String, Vector> load(ServletContext ctx) throws IOException {
        log.info("load - Started");

        FileReader in = null;
        String path = ctx.getRealPath("");
        log.info("RealPath: " + path);

        // load locale messages
        String locale= "_en";
        Map<String, Properties> localeMessages = new HashMap<>();
        try {
            //in = new FileInputStream("messages.properties");
            in = new FileReader( new ClassPathResource("messages.properties").getFile());
            Properties props = new Properties();
            props.load(in);
            in.close();
            localeMessages.put(locale, props);
        }
        catch (Exception ex) {
            log.error("Cannot locale message file: "+ex);
            try {
                assert in != null;
                in.close();
            }
            catch(IOException ignored) {
            }
        }

        ctx.setAttribute("localeMessages", localeMessages);
        //log.info("setAttribute - localeMessages: " + localeMessages);
        LocaleMessage.init(ctx);

        // load renders properties
        String nlatPath = path + File.separatorChar + "nlat";
        try {
            in = new FileReader(new ClassPathResource("render2nlat.properties").getFile());
            Properties props = new Properties();
            props.load(in);
            in.close();
            ctx.setAttribute("nlatRenders", props);

            Properties rev_props = new Properties();
            Iterator data=props.entrySet().iterator();
            Map.Entry item;
            while(data.hasNext())
            {
                item = (Map.Entry<String,String>)data.next();
                rev_props.put(item.getValue(), item.getKey());
            }
            ctx.setAttribute("nlatRendersRev", rev_props);
        }
        catch (Exception ex) {
            log.error("Cannot load render2nlat.properties file: "+ex);
            try {
                in.close();
            }
            catch(IOException e) {
            }
        }

        LinkedHashMap<String,Vector> map = new LinkedHashMap<>();
        //HashMap<String,Vector> map = new HashMap();
        //File dir = new File(nlatPath);
        File dir = new ClassPathResource("nlat").getFile();
        loadNlats(map, dir);

        File[] folders = dir.listFiles(new FileFilter() {
            @Override
            public boolean accept (File pathname) {
                return pathname.isDirectory();
            }
        });
        assert folders != null;
        Arrays.stream(folders).map(File::getName).forEach(log::info);
        for(File subDir : folders)
            log.info("loadNlats from " + subDir.getName()); //loadNlats(map, subDir);


        ctx.setAttribute("nlaTemplates", map);
        log.info("load - Completed");

        return map;


    }


    private void loadNlats(LinkedHashMap<String,Vector> map, File dir) {
        //private void loadNlats(HashMap<String,Vector> map, File dir) {
        log.info("loadNlats from " + dir.getName());
        String[] fileNames = dir.list(new NlatFilenameFilter());
        if (fileNames==null) {
            log.error("no NLA templates found");
            return;
        }

        Arrays.sort(fileNames);

        String folder = dir.getName();
        if (folder.equals("nlat"))
            folder = null;

        String key;
        String name;
        Vector data;
        String fileName;
        int ind;
        for (String s : fileNames) {
            fileName = s;
            if (folder != null)
                fileName = folder + "/" + s;

            ind = s.indexOf(".");
            name = s.substring(ind + 1);
            if (!name.equals("nlat"))
                key = s.substring(0, ind);
            else {
                key = "General";
                name = s;
            }
            name = name.replaceFirst("\\.nlat", "");
            String[] value = new String[]{name, fileName};

            data = map.get(key);
            if (data == null) {
                data = new Vector();
            }

            log.debug("loadNlats - Add new " + key + "<" + name + "," + fileName + ">");
            data.add(value);
            map.put(key, data);
        }
    }


/*
	// load general NLA data, such as templates list, translation table
	public void load(ServletContext ctx) {
            log.info("load - Started");

            FileInputStream in = null;
            String path = ctx.getRealPath("");

            // load locale messages
            String confPath = path + File.separatorChar + "WEB-INF" + File.separatorChar + "conf";
            File dir = new File(confPath);
            String[] fileNames = dir.list(new MessagesFilenameFilter());
            if (fileNames==null)
                log.error("no locale message files found");

            String locale;
            Map<String,Properties> localeMessages = new HashMap<>();
            try {
                for (int i = 0; i < Objects.requireNonNull(fileNames).length ; i++) {
                    locale = "_en";
                    int ind = fileNames[i].indexOf("_");
                    if (ind>0) {
                        int ind2 = fileNames[i].indexOf(".",ind+1);
                        locale = fileNames[i].substring(ind,ind2);
                    }

                    in = new FileInputStream(confPath+File.separatorChar+fileNames[i]);
                    Properties props = new Properties();
                    props.load(in);
                    in.close();
                    localeMessages.put(locale, props);
                }
            }
            catch (Exception ex) {
                log.error("Cannot locale message file: "+ex);
                try {
                    assert in != null;
                    in.close();
                }
                catch(IOException ignored) {
                }
            }
		
            ctx.setAttribute("localeMessages", localeMessages);
            //log.info("setAttribute - localeMessages: " + localeMessages);
            LocaleMessage.init(ctx);

            // load renders properties
            String nlatPath = path + File.separatorChar + "nlat";
            try {
                in = new FileInputStream(nlatPath+File.separatorChar+"render2nlat.properties");
                Properties props = new Properties();
                props.load(in);
                in.close();
                getServletContext().setAttribute("nlatRenders", props);

                Properties rev_props = new Properties();
                Iterator data=props.entrySet().iterator();
                Entry item;
                while(data.hasNext())
                {	
                    item = (Entry<String,String>)data.next();
                    rev_props.put(item.getValue(), item.getKey());
                }
                getServletContext().setAttribute("nlatRendersRev", rev_props);
            }
            catch (Exception ex) {
                log.error("Cannot load render2nlat.properties file: "+ex);
                try {
                    in.close();
                }
                catch(IOException e) {
                }
            }

            LinkedHashMap<String,Vector> map = new LinkedHashMap<>();
            //HashMap<String,Vector> map = new HashMap();
            dir = new File(nlatPath);
            loadNlats(map, dir);

            File[] folders = dir.listFiles(new FoldersFilter());
            assert folders != null;
            for(File subDir : folders)
                loadNlats(map, subDir);

            getServletContext().setAttribute("nlaTemplates", map);
            
            log.info("load - Completed");
	}
        

	private void loadNlats(LinkedHashMap<String,Vector> map, File dir) {
	//private void loadNlats(HashMap<String,Vector> map, File dir) {
            log.info("loadNlats from " + dir.getName());
            String[] fileNames = dir.list(new NlatFilenameFilter());
            if (fileNames==null) {
                log.error("no NLA templates found");
                return;
            }
            
            Arrays.sort(fileNames);

            String folder = dir.getName();
            if (folder.equals("nlat"))
                folder = null;

            String key;
            String name;
            Vector data;
            String fileName;
            int ind;
            for (String s : fileNames) {
                fileName = s;
                if (folder != null)
                    fileName = folder + "/" + s;

                ind = s.indexOf(".");
                name = s.substring(ind + 1);
                if (!name.equals("nlat"))
                    key = s.substring(0, ind);
                else {
                    key = "General";
                    name = s;
                }
                name = name.replaceFirst("\\.nlat", "");
                String[] value = new String[]{name, fileName};

                data = map.get(key);
                if (data == null) {
                    data = new Vector();
                }

                log.debug("loadNlats - Add new " + key + "<" + name + "," + fileName + ">");
                data.add(value);
                map.put(key, data);
            }
	}
*/
        
	private static boolean connectToActive(String active_passive_str) {
            String v[] = active_passive_str.split(",");
            
            if( (v[4].equals("SSO_ENABLE")) )
                SSO_flag=true;
            else if( (v[4].equals("SSO_DISABLE")) )
                SSO_flag=false;
                               
            if( (v[5].equals("DP_APP_CENTER:DP_ENABLE")) )
                DP_APP_CENTER=true;
            else if( (v[5].equals("DP_APP_CENTER:DP_DISABLE")) )
                DP_APP_CENTER=false;
            
            if( connectedServer == PRIMARY_SERVER &&
                (v[1].equals("ACTIVE")) || (v[1].equals("ACTIVE_SA")) )
                return true;
            else if( connectedServer == SECONDARY_SERVER &&
                (v[3].equals("ACTIVE")) || (v[3].equals("ACTIVE_SA")) )
                return true;
            
            return false;
	}
        
	public synchronized static int getConnectedServer() {
		return connectedServer;
	}

    /** 
     * Handles the HTTP <code>GET</code> method.
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
	    log.info("NLA templates rescan request");
	    load(request.getSession().getServletContext());
	    response.sendRedirect("openDialog.jsp");
    }


    private class FoldersFilter implements FileFilter {
		@Override
		public boolean accept (File pathname) {
			return pathname.isDirectory();
		}
    }

    private class NlatFilenameFilter implements  FilenameFilter {
		@Override
	    public boolean accept(File dir, String name) {
		    return name.endsWith(".nlat");
	    }
    }

    private class MessagesFilenameFilter implements  FilenameFilter {
		@Override
	    public boolean accept(File dir, String name) {
		    return name.startsWith("messages") && name.endsWith(".properties");
	    }
    }


    private class KeepAliveTask extends TimerTask {
		@Override
		public void run () {
			keepAlive();
		}
	}
}
