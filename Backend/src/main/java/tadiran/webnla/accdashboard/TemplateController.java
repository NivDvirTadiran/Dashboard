package tadiran.webnla.accdashboard;

import jakarta.servlet.ServletContext;
import jakarta.servlet.ServletException;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotNull;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.core.io.ClassPathResource;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseCookie;
import org.springframework.http.ResponseEntity;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.web.bind.annotation.*;

import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import tadiran.emisweb.*;
import tadiran.webnla.AbsCollection;
import tadiran.webnla.NLAContext;
import tadiran.webnla.accdashboard.payload.request.*;
import tadiran.webnla.accdashboard.payload.response.*;
import tadiran.webnla.bean.Agent;
import tadiran.webnla.bean.SessionData;
import tadiran.webnla.config.PropertiesManager;
import tadiran.webnla.endpoint.EMISwebServiceClient;
import tadiran.webnla.servlet.NLAService;
import tadiran.webnla.tag.LocaleMessage;
import tadiran.webnla.utils.Utils;

import java.io.*;
import java.net.MalformedURLException;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static tadiran.webnla.filter.AuthTokenFilter.getUserNameFromJwtToken;
import static tadiran.webnla.utils.Utils.verifyRequesterIP;

@CrossOrigin(origins = "*", maxAge = 3600)
@RestController
@RequestMapping("/auth")
public class TemplateController {
    protected final Logger logger = LogManager.getLogger(this.getClass());

    protected static EMISwebPort wsEMIS_port;
    private   static int updater_connected_server = NLAService.NOT_CONNECTED;

    private static final Logger log = LogManager.getLogger("TemplateController");
    private static String appId;
    private LoginRequestType loginData = new LoginRequestType();
    protected RequestInfoHeaderType reqHeader = new RequestInfoHeaderType();
    //protected AbsCollection collection;
    protected HashMap collectionData;
    LoginReturnType loginResp = null;
    long supLevel;
    private GenRequestType reqData=new GenRequestType();

    AuthenticationManager authenticationManager;

    static {
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
                    LogManager.getLogger(TemplateController.class).warn("WARNING, NLAService NOT Connected");
                    break;
            }
        } catch (MalformedURLException ex) {
            LogManager.getLogger(TemplateController.class).fatal("fatal error, cannot access web services: " + ex.getMessage());
        }
    }

    public TemplateController() {
    }

    @GetMapping("/check")
    @ResponseStatus(value = HttpStatus.OK)
    public String doPing(HttpServletResponse response) {
        return "{\"OK\":\"OK\"}";
    }



    @GetMapping("/Agents.Brief_Report.nlat")
    public ResponseEntity<?> table2(@RequestParam(required = false) Map<String, String> queryMap) {
        String agentIds = queryMap.get("agentId");
        Stream<Integer> ids = null;
        if (agentIds != null) {
            ids = Arrays.stream(agentIds.split(",")).map(Integer::parseInt);
            // Process the agent IDs here
            logger.info("agentIds: " + agentIds);
        } else {
            //return new ResponseEntity<>("Agent IDs are required", HttpStatus.BAD_REQUEST);
        }

        BriefInfoRequestType reqData2 = new BriefInfoRequestType();
        reqData2.setRequestInfoHeader(this.reqHeader);

        ReturnMatrixDataTypeBG result = null;
        if(wsEMIS_port == null) {
            try {
                wsEMIS_port = establishServerConnection();
            } catch (MalformedURLException e) {
                throw new RuntimeException(e);
            }
        }

        if (wsEMIS_port != null)
            result = wsEMIS_port.briefAgents(reqData2);
        else
            return new ResponseEntity<>("Service not available", HttpStatus.INTERNAL_SERVER_ERROR);

        List<DataItemRow> dataItemRow = result.getReturnMatrix();


        // Check if we have data before processing
        /*if (dataItemRow == null || dataItemRow.isEmpty()) {
            logger.warn("No agent data returned from service");
            return new ResponseEntity<>(Collections.emptyList(), HttpStatus.OK);
        }*/

        assert ids != null;
        @NotNull Set<Map> ans = dataItemRow.stream()
                .map(DataItemRow::getReturnArray)
                .flatMap(dataItem -> dataItem.stream().map(DataItemType::getValue))
                .filter(row -> row != null && !row.isEmpty())
                .map(row -> Arrays.stream(row.split(","))
                        .map(item -> item.replaceAll("(.*);f(.*);(.*);(.*)", "$3: $4"))
                        .map(item -> {
                                Map<String, String> map = new LinkedHashMap<>();
                                String[] parts = item.split(":", 2);
                                String key = parts[0].trim().replaceAll(" ", "");
                                String val = parts[1].trim();
                                map.put(key, val);
                            return map;
                        }).collect(Collectors.toMap(
                                map -> map.keySet().iterator().next(), // Extract the key from the map
                                map -> map.values().iterator().next(), // Extract the value from the map
                                (existing, replacement) -> existing, // Handle duplicate keys by keeping the existing value
                                LinkedHashMap::new // Use LinkedHashMap to maintain insertion order
                        )))
                .collect(Collectors.toSet());

        return new ResponseEntity<>(ans, HttpStatus.OK);
    }


    private Utils utils = new Utils();

    @GetMapping("/Agents.Brief_Report")
    public ResponseEntity<?> table() throws IOException {
        if (this.reqHeader == null || this.reqHeader.getTicket() == null || this.reqHeader.getTicket().isEmpty()) {
            return new ResponseEntity<>("User not authenticated or session expired.", HttpStatus.UNAUTHORIZED);
        }

        BriefInfoRequestType reqData2 = new BriefInfoRequestType();
        reqData2.setRequestInfoHeader(this.reqHeader);

        ReturnMatrixDataTypeBG result;
        try {
            if (wsEMIS_port == null) {
                wsEMIS_port = establishServerConnection();
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            }
            result = wsEMIS_port.briefAgents(reqData2);
        } catch (Exception e) {
            logger.error("Error calling briefAgents SOAP operation in table()", e);
            wsEMIS_port = null;
            return new ResponseEntity<>("Failed to fetch brief agents data: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (result == null || result.getResponseInfoHeader() == null) {
            return new ResponseEntity<>("Received null or invalid response from SOAP service for briefAgents (table)", HttpStatus.INTERNAL_SERVER_ERROR);
        }
        if (result.getResponseInfoHeader().getErrorCause() != 0) {
            logger.error("SOAP error in briefAgents response (table): " + result.getResponseInfoHeader().getErrorCause());
            return new ResponseEntity<>("SOAP service error (table): " + result.getResponseInfoHeader().getErrorCause(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        List<DataItemRow> dataItemRowList = result.getReturnMatrix();
        if (dataItemRowList == null) {
            logger.warn("BriefAgents (table) returned null for ReturnMatrix.");
            return new ResponseEntity<>(Collections.emptyList(), HttpStatus.OK);
        }

        for (DataItemRow dataItemRow : dataItemRowList) {
            if (dataItemRow != null && dataItemRow.getReturnArray() != null) {
                Agent agent = new Agent();
                agent.updateData(dataItemRow.getReturnArray());
            }
        }

        logger.info(result.getResponseInfoHeader().toString());

        ArrayList<String> newRowsJson = new ArrayList<>();
        result.getReturnMatrix().forEach(dataItemRow1 -> {
            dataItemRow1.getReturnArray().forEach(dataItem1 -> {
                String row = dataItem1.getValue();
                String[] items = row.split(",");
                Map<String, String> newItems = new LinkedHashMap<>();
                Arrays.stream(items).forEach(item -> {
                    String newItem = item.replaceAll("(.*);f(.*);\\s*(.*);(.*)", "$3: $4")
                            .replaceAll("\\s*:(.*)", ":$1")
                            .trim();
                    String key = "";
                    String val = "";
                    if (newItem.contains(":")) {
                        String[] parts = newItem.split(":", 2);
                        key = parts[0].trim().replaceAll(" ", "");
                        val = parts[1].trim();
                    }
                    if (!key.isEmpty()) {
                        newItems.put(key, val);
                    }
                });
                String newRow = utils.toJson(newItems);
                if (!newRow.isEmpty() && !"{}".equals(newRow)) {
                    log.info(newRow);
                    newRowsJson.add(newRow);
                }
            });
        });
        return new ResponseEntity<>(newRowsJson, HttpStatus.OK);
    }

    public LinkedHashMap<String, Vector<String[]>> load(ServletContext ctx) throws IOException {
        logger.info("load - Started");
        String path = ctx.getRealPath("");
        logger.info("RealPath: " + path);

        String locale = "_en";
        Map<String, Properties> localeMessages = new HashMap<>();
        try (InputStream is = new ClassPathResource("messages.properties").getInputStream()) {
            Properties props = new Properties();
            props.load(is);
            localeMessages.put(locale, props);
        } catch (Exception ex) {
            logger.error("Cannot load locale message file: messages.properties", ex);
        }

        ctx.setAttribute("localeMessages", localeMessages);
        LocaleMessage.init(ctx);

        try (InputStream is = new ClassPathResource("render2nlat.properties").getInputStream()) {
            Properties props = new Properties();
            props.load(is);
            ctx.setAttribute("nlatRenders", props);

            Properties rev_props = new Properties();
            for (Map.Entry<Object, Object> entry : props.entrySet()) {
                rev_props.put(entry.getValue(), entry.getKey());
            }
            ctx.setAttribute("nlatRendersRev", rev_props);
        } catch (Exception ex) {
            logger.error("Cannot load render2nlat.properties file", ex);
        }

        LinkedHashMap<String, Vector<String[]>> map = new LinkedHashMap<>();
        File dir = new ClassPathResource("nlat").getFile();
        loadNlats(map, dir);

        File[] folders = dir.listFiles(pathname -> pathname.isDirectory());
        if (folders != null) {
            Arrays.stream(folders).map(File::getName).forEach(logger::info);
            for (File subDir : folders) {
                logger.info("loadNlats from " + subDir.getName());
            }
        }

        ctx.setAttribute("nlaTemplates", map);
        logger.info("load - Completed");
        return map;
    }

    private void loadNlats(LinkedHashMap<String, Vector<String[]>> map, File dir) {
        logger.info("loadNlats from " + dir.getName());
        String[] fileNames = dir.list(new NlatFilenameFilter());
        if (fileNames == null) {
            logger.warn("no NLA templates found in " + dir.getAbsolutePath());
            return;
        }
        Arrays.sort(fileNames);
        String folder = dir.getName();
        if (folder.equals("nlat"))
            folder = null;

        for (String s : fileNames) {
            String fileName = folder != null ? folder + "/" + s : s;
            int ind = s.indexOf(".");
            String name = s.substring(ind + 1).replaceFirst("\\.nlat", "");
            String key = name.equals("nlat") ? "General" : s.substring(0, ind);
            String[] value = new String[]{name, fileName};

            map.computeIfAbsent(key, k -> new Vector<>()).add(value);
            logger.debug("loadNlats - Add new " + key + "<" + name + "," + fileName + ">");
        }
    }

    public class NlatFilenameFilter implements FilenameFilter {
        @Override
        public boolean accept(File dir, String name) {
            return name.endsWith(".nlat");
        }
    }


    //EMISwebPort wsEMIS_port = null;

    @CrossOrigin(origins = "https://localhost:4200", allowedHeaders = "Requestor-Type", exposedHeaders = "X-Get-Header")
    @PostMapping("/signin")
    public ResponseEntity<?> authenticateUser(@Valid @RequestBody LoginRequest loginRequest) {
        if (appId == null)
            appId = ((String) PropertiesManager.getProperty("tadiran.dashboard.appID", String.class));

        String langName = loginRequest.getLang();
        LoginRequestType currentLoginData = new LoginRequestType();

        try {
            String user = loginRequest.getUsername();
            String pass = loginRequest.getPassword();
            String sessionId = "70E8A04C55D4358D533DCDE1EBDBE4F7";
            String ipAddress = verifyRequesterIP();

            log.info("login request: -USER=" + user +
                    "\n (encoded " + URLEncoder.encode(user, StandardCharsets.UTF_8) + ")" +
                    "\n PASS=" + pass + "," +
                    "\n Language=" + langName + "," +
                    "\n session=" + sessionId + "," +
                    "\n IP Addr=" + ipAddress);

            if (user.startsWith("GateUser: ") && getUserNameFromJwtToken(pass).equals(user.substring(10))) {
                log.info("gate password validated");
                user = user.substring(10);
                pass = "userGate";
            } else if (pass.equals("userGate")) {
                pass = "PasswordForbidden";
            }


            loginData=new LoginRequestType();
            loginData.setServiceReqId(1);
            loginData.setApplicationId(appId);
            loginData.setName(URLEncoder.encode(user, StandardCharsets.UTF_8));
            loginData.setPass(pass);
            loginData.setSessionId(sessionId);
            loginData.setIpAddress(ipAddress);

            this.loginResp = null;

            if(wsEMIS_port == null) {  wsEMIS_port = establishServerConnection();  }

            //NLAContext.getInstance().invalidateOldUserSession(loginData);

            if(wsEMIS_port != null)
            {
                log.info("Send userLogin request for session_id " + sessionId);
                loginResp = wsEMIS_port.userLogin(loginData);
                if (loginResp.getLoginEnable() !=0 )
                {
                    log.info("LOGIN Success (session_id " + sessionId + ")");
                    this.reqHeader.setServiceReqId(1);
                    this.supLevel = (int) loginResp.getSupLevel();
                    this.reqHeader.setTicket( loginResp.getTicket());
                    this.reqData.setRequestInfoHeader(this.reqHeader);
                    //18-Mar-2019 YR BZ#49276
                    //Cookie userCookie = new Cookie("userName", URLEncoder.encode(user,"UTF-8"));
                    int COOKIE_LIFETIME_MIN = 24 * 60 * 60; // = one day long
                    ResponseCookie userCookie2 = ResponseCookie
                            .from("userName", URLEncoder.encode(user, StandardCharsets.UTF_8))
                            .maxAge(COOKIE_LIFETIME_MIN)
                            .httpOnly(false)
                            .build();
                    ResponseEntity<?> resp = ResponseEntity.ok()
                            .header(HttpHeaders.SET_COOKIE, userCookie2.toString())
                            .body(new LoginInfoResponse(
                                    user,
                                    sessionId)
                            );

                    log.info("Create new SessionData");
                    log.info("Create new SessionData");
                    SessionData sessionData = new SessionData();
                    if(!sessionData.isValid())
                    {
                        log.warn("sessionData Created is NOT VALID");
                    }
                    sessionData.init(loginData,loginResp);
                    NLAContext.getInstance().getSessions().add(sessionData);

                    return resp;
                }

            }

        }
        catch(Exception e) {
            log.error("Cannot process WebNLA service: "+e.getMessage(), e);
        }

        log.info(" Forward to LOGIN Page");
        //req.getRequestDispatcher("login.jsp").forward(req, resp);
        wsEMIS_port=null;
        return new ResponseEntity<>( "loginError", HttpStatus.FORBIDDEN);

    }


    @PostMapping("/signout")
    public ResponseEntity<?> Signout() {
        try {
            logger.info("Sign out started");
            ResponseInfoHeaderType responseInfoHeaderType = wsEMIS_port.userLogout(this.reqData);
            logger.info("Successfully signed out: " + responseInfoHeaderType.getServersInfo());
            return ResponseEntity.ok()
                    .body(new ResponseMessage("You've been signed out!"));
        } catch (RuntimeException e) {
            logger.info("Singed out failed - Error: " + e.getMessage());
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    "Singed out failed!"));
        }

        /*Authentication authentication = SecurityContextHolder.getContext().getAuthentication();
        AgentDetailsImpl supDetails = (AgentDetailsImpl) authentication.getPrincipal();
        String sessionId = supDetails.getSessionId();


        try {
            agentDetailsService.releaseSessionLicence(sessionId);

            if (AccApiConfig.isAuthTokenFilterOn)
                return ResponseEntity.ok()
                        .body(new ResponseMessage("You've been signed out!"));
            else {
                ResponseCookie cookie = jwtUtils.getCleanJwtCookie();
                return ResponseEntity.ok()
                        .header(HttpHeaders.SET_COOKIE, cookie.toString())
                        .body(new ResponseMessage("You've been signed out!"));
            }
        } catch (RuntimeException e) {
            logger.info("Singed out failed - Error: " + e.getMessage());
            return ResponseEntity.badRequest().body(new ResponseMessage(
                    "Singed out failed!"));
        } finally {
            SecurityContextHolder.clearContext();
        }*/
    }

    @PostMapping("/login")
    public ResponseEntity<?> Login2(HttpServletRequest req) throws ServletException, IOException {
        if (appId == null)
            appId = ((String) PropertiesManager.getProperty("tadiran.dashboard.appID", String.class));

        String langName = req.getParameter("lang");
        req.setCharacterEncoding("UTF-8");
        LoginRequestType currentLoginData = new LoginRequestType();

        try {
            String user = req.getParameter("username");
            String pass = req.getParameter("password");
            if (user == null || pass == null) {
                return new ResponseEntity<>("Username and password are required.", HttpStatus.BAD_REQUEST);
            }
            String sessionId = req.getSession().getId();
            String ipAddress = getClientIpAddress(req);

            log.info("login request (Login2): USER=" + user +
                    "\n (encoded " + URLEncoder.encode(user, StandardCharsets.UTF_8) + ")" +
                    "\n PASS=" + pass + "," +
                    "\n Language=" + langName + "," +
                    "\n session=" + sessionId + "," +
                    "\n IP Addr=" + ipAddress);

            if (user.startsWith("GateUser: ") && getUserNameFromJwtToken(pass).equals(user.substring(10))) {
                log.info("gate password validated");
                user = user.substring(10);
                pass = "userGate";
            } else if (pass.equals("userGate")) {
                pass = "PasswordForbidden";
            }

            currentLoginData.setServiceReqId(1);
            currentLoginData.setApplicationId(appId);
            currentLoginData.setName(URLEncoder.encode(user, StandardCharsets.UTF_8));
            currentLoginData.setPass(pass);
            currentLoginData.setSessionId(sessionId);
            currentLoginData.setIpAddress(ipAddress);

            LoginReturnType currentLoginResp = null;
            if (wsEMIS_port == null) {
                wsEMIS_port = establishServerConnection();
            }

            if (wsEMIS_port != null) {
                log.info("Send userLogin request for session_id " + sessionId);
                currentLoginResp = wsEMIS_port.userLogin(currentLoginData);
                if (currentLoginResp != null && currentLoginResp.getLoginEnable() != 0) {
                    log.info("LOGIN Success (session_id " + sessionId + " via Login2)");
                    this.reqHeader.setServiceReqId(1);
                    this.supLevel = (int) currentLoginResp.getSupLevel();
                    this.reqHeader.setTicket(currentLoginResp.getTicket());

                    int COOKIE_LIFETIME_MIN = 24 * 60 * 60;
                    ResponseCookie userCookie2 = ResponseCookie
                            .from("userName", URLEncoder.encode(user, StandardCharsets.UTF_8))
                            .maxAge(COOKIE_LIFETIME_MIN)
                            .httpOnly(false)
                            .path("/")
                            .secure(true)
                            .sameSite("None")
                            .build();
                    ResponseEntity<?> resp = ResponseEntity.ok()
                            .header(HttpHeaders.SET_COOKIE, userCookie2.toString()).body(user);
                    log.info("Create new SessionData (Login2)");
                    return resp;
                } else {
                    log.warn("Login2 failed or loginResp is null. Enable: " + (currentLoginResp != null ? currentLoginResp.getLoginEnable() : "null"));
                }
            }
        } catch (Exception e) {
            log.error("Cannot process WebNLA service (Login2): " + e.getMessage(), e);
            req.setAttribute("loginError", LocaleMessage.getMessage("login.InvSrv", langName));
        }
        log.info(" Forward to LOGIN Page (Login2) - Login Failed");
        wsEMIS_port = null;
        this.reqHeader = new RequestInfoHeaderType();
        return new ResponseEntity<>("loginError", HttpStatus.UNAUTHORIZED);
    }

    public static EMISwebPort establishServerConnection() throws MalformedURLException {
        if (wsEMIS_port != null) {
            log.info("wsEMIS_port already exists, re-using.");
        }

        switch (NLAService.getConnectedServer()) {
            case NLAService.PRIMARY_SERVER:
                log.info("wsEMIS_port establishing/re-establishing with PRIMARY_SERVER");
                wsEMIS_port = new EMISwebServiceClient(NLAContext.ACCServerAddress1, NLAContext.ACCServerPort1).getEMISwebPort();
                break;
            case NLAService.SECONDARY_SERVER:
                log.info("wsEMIS_port establishing/re-establishing with SECONDARY_SERVER");
                wsEMIS_port = new EMISwebServiceClient(NLAContext.ACCServerAddress2, NLAContext.ACCServerPort2).getEMISwebPort();
                break;
            default:
                log.warn("wsEMIS_port cannot be established, NLAService NOT connected");
                wsEMIS_port = null;
                break;
        }
        return wsEMIS_port;
    }

    public static String getClientIpAddress(HttpServletRequest req) {
        for (String header : HEADERS_TO_TRY) {
            String ip = req.getHeader(header);
            if (ip != null && !ip.isEmpty() && !"unknown".equalsIgnoreCase(ip)) {
                if ("0:0:0:0:0:0:0:1".equals(ip) || "::1".equals(ip))
                    return "127.0.0.1";
                else
                    return ip;
            }
        }
        String remoteAddr = req.getRemoteAddr();
        if ("::1".equals(remoteAddr) || "0:0:0:0:0:0:0:1".equals(remoteAddr)) return "127.0.0.1";
        return remoteAddr;
    }

    private static final String[] HEADERS_TO_TRY = {
            "X-Forwarded-For",
            "Proxy-Client-IP",
            "WL-Proxy-Client-IP",
            "HTTP_X_FORWARDED_FOR",
            "HTTP_X_FORWARDED",
            "HTTP_X_CLUSTER_CLIENT_IP",
            "HTTP_CLIENT_IP",
            "HTTP_FORWARDED_FOR",
            "HTTP_FORWARDED",
            "HTTP_VIA",
            "REMOTE_ADDR"};

    @GetMapping("/api/soap/AgentsList")
    public ResponseEntity<?> getAgentsListProxy() {
        logger.info("Received request for getAgentsListProxy");
        if (!isUserAuthenticated()) {
            return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
        }

        AgentsListReturnType response = wsEMIS_port.agentsList(this.reqData);

        if (response == null || response.getResponseInfoHeader() == null) {
            logger.error("Received null or invalid response from SOAP service for AgentsList");
            return new ResponseEntity<>("Invalid response from SOAP service for AgentsList", HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (response.getResponseInfoHeader().getErrorCause() != 0) {
            logger.error("SOAP error in AgentsList response: " + response.getResponseInfoHeader().getErrorCause() + " - " + response.getResponseInfoHeader().getServersInfo());
            return new ResponseEntity<>("SOAP service error in AgentsList: " + response.getResponseInfoHeader().getErrorCause() + " - " + response.getResponseInfoHeader().getServersInfo(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        logger.info("Successfully fetched AgentsList. Returning " + (response.getReturnArray() != null ? response.getReturnArray().size() : 0) + " items.");
        return ResponseEntity.ok(response);
    }

    @GetMapping("/api/soap/GroupList")
    public ResponseEntity<?> getGroupListProxy() {
        logger.info("Received request for getGroupListProxy");
        if (!isUserAuthenticated()) {
            return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
        }

        // Assuming GroupList operation also uses GenRequestType (this.reqData)
        // as per WSDL element <xsd:element name="RequestGroupList" type="tns:GenRequestType"></xsd:element>
        GroupListReturnType response = wsEMIS_port.groupList(this.reqData);

        if (response == null || response.getResponseInfoHeader() == null) {
            logger.error("Received null or invalid response from SOAP service for GroupList");
            return new ResponseEntity<>("Invalid response from SOAP service for GroupList", HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (response.getResponseInfoHeader().getErrorCause() != 0) {
            logger.error("SOAP error in GroupList response: " + response.getResponseInfoHeader().getErrorCause() + " - " + response.getResponseInfoHeader().getServersInfo());
            return new ResponseEntity<>("SOAP service error in GroupList: " + response.getResponseInfoHeader().getErrorCause() + " - " + response.getResponseInfoHeader().getServersInfo(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        logger.info("Successfully fetched GroupList. Returning " + (response.getReturnArray() != null ? response.getReturnArray().size() : 0) + " items.");
        return ResponseEntity.ok(response);
    }

    private boolean isUserAuthenticated() {
        return this.reqHeader != null && this.reqHeader.getTicket() != null && !this.reqHeader.getTicket().isEmpty();
    }

    @GetMapping("/api/soap/brief-agents")
    public ResponseEntity<?> getBriefAgentsProxy(@RequestParam(name = "agentIds", required = false) String agentIdsParam) {
        logger.info("Received request for BriefAgents proxy. Requested agentIds: {}", agentIdsParam);
        try {
            if (wsEMIS_port == null) {
                wsEMIS_port = establishServerConnection();
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            }
            if (this.reqHeader == null || this.reqHeader.getTicket() == null || this.reqHeader.getTicket().isEmpty()) {
                logger.warn("RequestInfoHeader is null or ticket is missing for BriefAgents. Client needs to re-authenticate.");
                return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
            }

            BriefInfoRequestType request = new BriefInfoRequestType();
            request.setRequestInfoHeader(this.reqHeader);
            ReturnMatrixDataTypeBG response = wsEMIS_port.briefAgents(request);

            if (response == null || response.getResponseInfoHeader() == null) {
                logger.error("Received null or invalid response from SOAP service for BriefAgents");
                return new ResponseEntity<>("Invalid response from SOAP service for BriefAgents", HttpStatus.INTERNAL_SERVER_ERROR);
            }
            if (response.getResponseInfoHeader().getErrorCause() != 0) {
                logger.error("SOAP error in BriefAgents response: {} Info: {}", response.getResponseInfoHeader().getErrorCause(), response.getResponseInfoHeader().getServersInfo());
                return new ResponseEntity<>("SOAP service error (BriefAgents): " + response.getResponseInfoHeader().getErrorCause(), HttpStatus.INTERNAL_SERVER_ERROR);
            }

            // Filter if agentIdsParam is provided
            if (agentIdsParam != null && !agentIdsParam.isEmpty()) {
                Set<String> requestedAgentIds = Arrays.stream(agentIdsParam.split(","))
                                                     .map(String::trim)
                                                     .collect(Collectors.toSet());
                if (!requestedAgentIds.isEmpty()) {
                    logger.info("Filtering BriefAgents data for agent IDs: {}", requestedAgentIds);
                    List<DataItemRow> filteredMatrix = response.getReturnMatrix().stream()
                        .filter(dataItemRow -> {
                            if (dataItemRow == null || dataItemRow.getReturnArray() == null || dataItemRow.getReturnArray().isEmpty()) {
                                return false;
                            }
                            // Let's assume "Agent No." (ersname) or its corresponding value holds the ID.
                            // The ersid for "Agent No." might be "ER_AGENT_NUM" or similar.
                            // Example: value might be "xxx;fAgent No.;Agent No.;1001"
                            for (DataItemType item : dataItemRow.getReturnArray()) {
                                if (item != null && item.getErsid() != null && Objects.equals(item.getErsname(), "ALL") && item.getValue() != null) {
                                    logger.info("item(DataItemType): ersid-{}, ersname-{}, value-{}",
                                            item.getErsid(),item.getErsname(),item.getValue());
                                    // The value of field 'value' contain long string with many column,
                                    // one after another, that separated by the char symbol `,`.
                                    // Assuming the ID column value can be found in one of the columns objects
                                    // within the long string of field 'value'.
                                    // We need to identify which column content data represents the Key ID.
                                    String[] columns = item.getValue().split(",");
                                    String column = Arrays.stream(columns)
                                            .filter(col -> col.contains(item.getErsid()))
                                            .findFirst()
                                            .orElse(null);
                                    assert column != null;
                                    String[] fields = column.split(";");
                                    // fields[0] = ersId (e.g., 0)
                                    // fields[1] = "f" + ersname (e.g., f6_3_2_1_0)
                                    // fields[2] = ersname (e.g., Agent Id)
                                    // fields[3] = actual value (e.g., 1)
                                    if (fields.length >= 4) {
                                        String fieldId = fields[1];
                                        String fieldName = fields[2];
                                        String fieldValue = fields[3];
                                        // verify again that the fieldId matches the requested agent IDs
                                        if (item.getErsid().equalsIgnoreCase(fieldId) && requestedAgentIds.contains(fieldValue)) {
                                            return true; // This row matches one of the requested agent IDs
                                        }
                                    }
                                }
                            }
                            return false; // Row does not match any requested agent ID
                        })
                        .collect(Collectors.toList());
                    
                    ReturnMatrixDataTypeBG filteredResponse = new ReturnMatrixDataTypeBG();
                    filteredResponse.setResponseInfoHeader(response.getResponseInfoHeader());
                    filteredResponse.getReturnMatrix().addAll(filteredMatrix);
                    logger.info("Filtered BriefAgents. Original rows: {}, Filtered rows: {}", response.getReturnMatrix().size(), filteredMatrix.size());
                    return new ResponseEntity<>(filteredResponse, HttpStatus.OK);
                }
            }
            // If no filtering needed or agentIdsParam is empty, return original response
            return new ResponseEntity<>(response, HttpStatus.OK);
        } catch (Exception e) {
            logger.error("Error calling BriefAgents SOAP operation", e);
            wsEMIS_port = null; // Consider resetting on error
            return new ResponseEntity<>("Failed to fetch brief agents data: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }
    }

    // Updated method to get the currently stored request header
    private RequestInfoHeaderType getRequestInfoHeader() {
        // Check if the ticket is valid or placeholder
        return this.reqHeader;
    }

    @GetMapping("/api/soap/super-groups")
    public ResponseEntity<?> getSuperGroupList() {
        logger.info("Received request for /api/soap/super-groups");

        // Check if wsEMIS_port is initialized
        if (wsEMIS_port == null) {
            try {
                wsEMIS_port = TemplateController.establishServerConnection(); // Reuse connection logic
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            } catch (MalformedURLException e) {
                logger.error("Error establishing server connection", e);
                return new ResponseEntity<>("Internal server error: Could not connect to backend service.", HttpStatus.INTERNAL_SERVER_ERROR);
            }
        }

        // Obtain RequestInfoHeader
        RequestInfoHeaderType currentAuthHeader = getRequestInfoHeader();
        if (currentAuthHeader == null || currentAuthHeader.getTicket() == null || currentAuthHeader.getTicket().isEmpty() || "placeholder_ticket".equals(currentAuthHeader.getTicket())) {
            logger.warn("RequestInfoHeader is missing or has placeholder ticket for /super-groups. Authentication required.");
            return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
        }

        GenRequestType request = new GenRequestType();
        request.setRequestInfoHeader(currentAuthHeader);

        SuperGroupListReturnType soapResponse;
        try {
            soapResponse = wsEMIS_port.superGroupList(request);
        } catch (Exception e) {
            logger.error("Error calling superGroupList SOAP operation", e);
            // Consider resetting wsEMIS_port on certain errors
            return new ResponseEntity<>("Failed to fetch super group list: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse == null || soapResponse.getResponseInfoHeader() == null) {
            logger.error("Received null or invalid response from SOAP service for SuperGroupList");
            return new ResponseEntity<>("Invalid response from SOAP service for SuperGroupList", HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse.getResponseInfoHeader().getErrorCause() != 0) {
            logger.error("SOAP error in SuperGroupList response: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo());
            return new ResponseEntity<>("SOAP service error in SuperGroupList: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        // Map SOAP response to REST DTO
        SuperGroupListResponseDto responseDto = new SuperGroupListResponseDto();
        ResponseInfoHeaderDto responseHeaderDto = new ResponseInfoHeaderDto();
        responseHeaderDto.setServiceReqId(soapResponse.getResponseInfoHeader().getServiceReqId());
        responseHeaderDto.setErrorCause(soapResponse.getResponseInfoHeader().getErrorCause());
        responseHeaderDto.setServersInfo(soapResponse.getResponseInfoHeader().getServersInfo());
        responseDto.setResponseInfoHeader(responseHeaderDto);

        if (soapResponse.getReturnArray() != null) {
            List<SuperGroupListItemDto> superGroupListItems = new java.util.ArrayList<>();
            for (tadiran.emisweb.SuperGroupListDataItemType item : soapResponse.getReturnArray()) {
                SuperGroupListItemDto itemDto = new SuperGroupListItemDto();
                itemDto.setSuperGroupId(item.getSuperGroupId());
                itemDto.setSuperGroupName(item.getSuperGroupName());
                // Map nested GroupListDataItemType list using a for loop
                if (item.getReturnArray() != null) {
                    List<GroupListItemDto> groupListItems = new java.util.ArrayList<>();
                    for (tadiran.emisweb.GroupListDataItemType groupItem : item.getReturnArray()) {
                        GroupListItemDto groupItemDto = new GroupListItemDto();
                        groupItemDto.setGrpId(groupItem.getGrpId());
                        groupItemDto.setGrpName(groupItem.getGrpName());
                        groupItemDto.setGrpNumber(groupItem.getGrpNumber());
                        groupItemDto.setGrpEmailEnable(groupItem.getGrpEmailEnable());
                        groupListItems.add(groupItemDto);
                    }
                    itemDto.setReturnArray(groupListItems);
                } else {
                    itemDto.setReturnArray(java.util.Collections.emptyList());
                }
                superGroupListItems.add(itemDto);
            }
            responseDto.setReturnArray(superGroupListItems);
        } else {
            responseDto.setReturnArray(java.util.Collections.emptyList());
        }

        logger.info("Successfully fetched SuperGroupList. Returning " + (responseDto.getReturnArray() != null ? responseDto.getReturnArray().size() : 0) + " items.");
        return ResponseEntity.ok(responseDto);
    }



    @GetMapping("/api/soap/dnis-list")
    public ResponseEntity<?> getDnisList() {
        logger.info("Received request for /api/modernized/dnis-list");

        if (wsEMIS_port == null) {
            try {
                wsEMIS_port = TemplateController.establishServerConnection();
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            } catch (MalformedURLException e) {
                logger.error("Error establishing server connection for /dnis-list", e);
                return new ResponseEntity<>("Internal server error: Could not connect to backend service.", HttpStatus.INTERNAL_SERVER_ERROR);
            }
        }

        RequestInfoHeaderType currentAuthHeader = getRequestInfoHeader();
        if (currentAuthHeader == null || currentAuthHeader.getTicket() == null || currentAuthHeader.getTicket().isEmpty() || "placeholder_ticket".equals(currentAuthHeader.getTicket())) {
            logger.warn("RequestInfoHeader is missing or has placeholder ticket for /dnis-list. Authentication required.");
            return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
        }

        GenRequestType request = new GenRequestType();
        request.setRequestInfoHeader(currentAuthHeader);

        List<DnisListItemDto> combinedDnisList = new java.util.ArrayList<>();
        ResponseInfoHeaderType finalResponseHeader = null;

        // Call Voice DNIS List
        try {
            logger.debug("Calling voiceDNISList SOAP operation for combined DNIS list");
            DNISListReturnType voiceDnisResponse = wsEMIS_port.voiceDNISList(request);
            if (voiceDnisResponse != null && voiceDnisResponse.getResponseInfoHeader() != null) {
                finalResponseHeader = voiceDnisResponse.getResponseInfoHeader(); // Store header
                if (voiceDnisResponse.getResponseInfoHeader().getErrorCause() == 0) {
                    if (voiceDnisResponse.getReturnArray() != null) {
                        voiceDnisResponse.getReturnArray().forEach(item -> {
                            DnisListItemDto itemDto = new DnisListItemDto();
                            itemDto.setDnisId(item.getDnisId());
                            itemDto.setDnisName(item.getDnisName());
                            itemDto.setDnisNumber(item.getDnisNumber()); // Assuming DnisListItemDto has this
                            itemDto.setDnisType(item.getDnisType());     // Assuming DnisListItemDto has this
                            combinedDnisList.add(itemDto);
                        });
                    }
                } else {
                    logger.error("SOAP error in VoiceDNISList part of combined DnisList: {} - {}", finalResponseHeader.getErrorCause(), finalResponseHeader.getServersInfo());
                    // Decide if to proceed or fail here. For now, we'll try email list too.
                }
            } else {
                logger.error("Received null or invalid response from SOAP service for VoiceDNISList part of combined DnisList");
                // Potentially return error if this is critical
            }
        } catch (Exception e) {
            logger.error("Error calling voiceDNISList SOAP operation for combined DNIS list", e);
            return new ResponseEntity<>("Failed to fetch voice DNIS list part: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        // Call Email DNIS List
        try {
            logger.debug("Calling emailDNISList SOAP operation for combined DNIS list");
            DNISListReturnType emailDnisResponse = wsEMIS_port.emailDNISList(request);
            if (emailDnisResponse != null && emailDnisResponse.getResponseInfoHeader() != null) {
                if (finalResponseHeader == null) finalResponseHeader = emailDnisResponse.getResponseInfoHeader(); // Use if first call failed to provide header
                if (emailDnisResponse.getResponseInfoHeader().getErrorCause() == 0) {
                    if (emailDnisResponse.getReturnArray() != null) {
                        emailDnisResponse.getReturnArray().forEach(item -> {
                            DnisListItemDto itemDto = new DnisListItemDto();
                            itemDto.setDnisId(item.getDnisId());
                            itemDto.setDnisName(item.getDnisName());
                            itemDto.setDnisNumber(item.getDnisNumber()); // Assuming DnisListItemDto has this
                            itemDto.setDnisType(item.getDnisType());     // Assuming DnisListItemDto has this
                            combinedDnisList.add(itemDto); // Add to the same list
                        });
                    }
                } else {
                    logger.error("SOAP error in EmailDNISList part of combined DnisList: {} - {}", emailDnisResponse.getResponseInfoHeader().getErrorCause(), emailDnisResponse.getResponseInfoHeader().getServersInfo());
                    // If voice list also had an error, or this is the only error, propagate it.
                    if (finalResponseHeader == null || finalResponseHeader.getErrorCause() == 0) { // If voice was OK or no header yet
                        finalResponseHeader = emailDnisResponse.getResponseInfoHeader();
                    }
                }
            } else {
                logger.error("Received null or invalid response from SOAP service for EmailDNISList part of combined DnisList");
            }
        } catch (Exception e) {
            logger.error("Error calling emailDNISList SOAP operation for combined DNIS list", e);
            return new ResponseEntity<>("Failed to fetch email DNIS list part: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        DnisListResponseDto responseDto = new DnisListResponseDto();
        ResponseInfoHeaderDto responseHeaderDto = new ResponseInfoHeaderDto();

        if (finalResponseHeader != null) {
            responseHeaderDto.setServiceReqId(finalResponseHeader.getServiceReqId());
            responseHeaderDto.setErrorCause(finalResponseHeader.getErrorCause());
            responseHeaderDto.setServersInfo(finalResponseHeader.getServersInfo());
        } else {
            // Fallback if both calls failed to produce a header (e.g., connection issues before SOAP response)
            responseHeaderDto.setErrorCause(1); // Generic error
            responseHeaderDto.setServersInfo("Could not retrieve response header from backend services.");
            logger.error("Could not determine final response header for combined DnisList");
        }
        responseDto.setResponseInfoHeader(responseHeaderDto);
        responseDto.setReturnArray(combinedDnisList);

        // If there was an error in any of the calls reflected in finalResponseHeader
        if (finalResponseHeader != null && finalResponseHeader.getErrorCause() != 0) {
            logger.error("SOAP error in combined DnisList operation: {} - {}", finalResponseHeader.getErrorCause(), finalResponseHeader.getServersInfo());
            return new ResponseEntity<>(responseDto, HttpStatus.INTERNAL_SERVER_ERROR); // Return the DTO with error info
        }

        logger.info("Successfully fetched combined DnisList. Returning " + combinedDnisList.size() + " items.");
        return ResponseEntity.ok(responseDto);
    }

    @GetMapping("/api/soap/reports")
    public ResponseEntity<?> getReportList() {
        logger.info("Received request for /api/modernized/reports");

        // Check if wsEMIS_port is initialized
        if (wsEMIS_port == null) {
            try {
                wsEMIS_port = TemplateController.establishServerConnection(); // Reuse connection logic
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            } catch (MalformedURLException e) {
                logger.error("Error establishing server connection", e);
                return new ResponseEntity<>("Internal server error: Could not connect to backend service.", HttpStatus.INTERNAL_SERVER_ERROR);
            }
        }

        // Obtain RequestInfoHeader
        RequestInfoHeaderType currentAuthHeader = getRequestInfoHeader();
        if (currentAuthHeader == null || currentAuthHeader.getTicket() == null || currentAuthHeader.getTicket().isEmpty() || "placeholder_ticket".equals(currentAuthHeader.getTicket())) {
            logger.warn("RequestInfoHeader is missing or has placeholder ticket for /reports. Authentication required.");
            return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
        }

        ReportListDataRequestType request = new ReportListDataRequestType();
        request.setRequestInfoHeader(currentAuthHeader);

        ReportListDataReturnType soapResponse;
        try {
            soapResponse = wsEMIS_port.reportListData(request);
        } catch (Exception e) {
            logger.error("Error calling reportListData SOAP operation", e);
            // Consider resetting wsEMIS_port on certain errors
            return new ResponseEntity<>("Failed to fetch report list: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse == null || soapResponse.getResponseInfoHeader() == null) {
            logger.error("Received null or invalid response from SOAP service for ReportListData");
            return new ResponseEntity<>("Invalid response from SOAP service for ReportListData", HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse.getResponseInfoHeader().getErrorCause() != 0) {
            logger.error("SOAP error in ReportListData response: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo());
            return new ResponseEntity<>("SOAP service error in ReportListData: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        // Map SOAP response to REST DTO
        ReportListResponseDto responseDto = new ReportListResponseDto();
        ResponseInfoHeaderDto responseHeaderDto = new ResponseInfoHeaderDto();
        responseHeaderDto.setServiceReqId(soapResponse.getResponseInfoHeader().getServiceReqId());
        responseHeaderDto.setErrorCause(soapResponse.getResponseInfoHeader().getErrorCause());
        responseHeaderDto.setServersInfo(soapResponse.getResponseInfoHeader().getServersInfo());
        responseDto.setResponseInfoHeader(responseHeaderDto);

        if (soapResponse.getReturnArray() != null) {
            List<ReportListItemDto> reportListItems = soapResponse.getReturnArray().stream()
                    .map(item -> {
                        ReportListItemDto itemDto = new ReportListItemDto();
                        itemDto.setReportId(item.getReportId());
                        itemDto.setIsPublic(item.getIsPublic());
                        itemDto.setReportName(item.getReportName());
                        itemDto.setRenderName(item.getRenderName());
                        itemDto.setParams(item.getParams());
                        return itemDto;
                    })
                    .collect(Collectors.toList());
            responseDto.setReturnArray(reportListItems);
        } else {
            responseDto.setReturnArray(java.util.Collections.emptyList());
        }

        logger.info("Successfully fetched ReportListData. Returning " + (responseDto.getReturnArray() != null ? responseDto.getReturnArray().size() : 0) + " items.");
        return ResponseEntity.ok(responseDto);
    }

    @GetMapping("/api/soap/brief-dnis")
    public ResponseEntity<?> getVoiceDnisList() {
        logger.info("Received request for /api/modernized/brief-dnis");

        if (wsEMIS_port == null) {
            try {
                wsEMIS_port = TemplateController.establishServerConnection();
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            } catch (MalformedURLException e) {
                logger.error("Error establishing server connection for Voice DNIS List", e);
                return new ResponseEntity<>("Internal server error: Could not connect to backend service.", HttpStatus.INTERNAL_SERVER_ERROR);
            }
        }

        RequestInfoHeaderType currentAuthHeader = getRequestInfoHeader();
        if (currentAuthHeader == null || currentAuthHeader.getTicket() == null || currentAuthHeader.getTicket().isEmpty() || "placeholder_ticket".equals(currentAuthHeader.getTicket())) {
            logger.warn("RequestInfoHeader is missing or has placeholder ticket for /brief-dnis. Authentication required.");
            return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
        }

        GenRequestType request = new GenRequestType();
        request.setRequestInfoHeader(currentAuthHeader);

        DNISListReturnType soapResponse;
        try {
            soapResponse = wsEMIS_port.voiceDNISList(request);
        } catch (Exception e) {
            logger.error("Error calling voiceDNISList SOAP operation", e);
            wsEMIS_port = null; // Consider resetting port on error
            return new ResponseEntity<>("Failed to fetch voice DNIS list: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse == null || soapResponse.getResponseInfoHeader() == null) {
            logger.error("Received null or invalid response from SOAP service for VoiceDNISList");
            return new ResponseEntity<>("Invalid response from SOAP service for VoiceDNISList", HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse.getResponseInfoHeader().getErrorCause() != 0) {
            logger.error("SOAP error in VoiceDNISList response: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo());
            return new ResponseEntity<>("SOAP service error in VoiceDNISList: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        DnisListResponseDto responseDto = new DnisListResponseDto();
        ResponseInfoHeaderDto responseHeaderDto = new ResponseInfoHeaderDto();
        responseHeaderDto.setServiceReqId(soapResponse.getResponseInfoHeader().getServiceReqId());
        responseHeaderDto.setErrorCause(soapResponse.getResponseInfoHeader().getErrorCause());
        responseHeaderDto.setServersInfo(soapResponse.getResponseInfoHeader().getServersInfo());
        responseDto.setResponseInfoHeader(responseHeaderDto);

        if (soapResponse.getReturnArray() != null) {
            List<DnisListItemDto> dnisListItems = soapResponse.getReturnArray().stream()
                    .map(item -> {
                        DnisListItemDto itemDto = new DnisListItemDto();
                        itemDto.setDnisId(item.getDnisId());
                        itemDto.setDnisName(item.getDnisName());
                        itemDto.setDnisNumber(item.getDnisNumber());
                        itemDto.setDnisType(item.getDnisType());
                        return itemDto;
                    })
                    .collect(Collectors.toList());
            responseDto.setReturnArray(dnisListItems);
        } else {
            responseDto.setReturnArray(java.util.Collections.emptyList());
        }

        logger.info("Successfully fetched VoiceDNISList. Returning " + (responseDto.getReturnArray() != null ? responseDto.getReturnArray().size() : 0) + " items.");
        return ResponseEntity.ok(responseDto);
    }

    @GetMapping("/api/soap/brief-voice-dnis")
    public ResponseEntity<?> getBriefVoiceDnis() {
        logger.info("Received request for /api/modernized/brief-voice-dnis");

        if (wsEMIS_port == null) {
            try {
                wsEMIS_port = TemplateController.establishServerConnection();
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            } catch (MalformedURLException e) {
                logger.error("Error establishing server connection for Brief Voice DNIS", e);
                return new ResponseEntity<>("Internal server error: Could not connect to backend service.", HttpStatus.INTERNAL_SERVER_ERROR);
            }
        }

        RequestInfoHeaderType currentAuthHeader = getRequestInfoHeader();
        if (currentAuthHeader == null || currentAuthHeader.getTicket() == null || currentAuthHeader.getTicket().isEmpty() || "placeholder_ticket".equals(currentAuthHeader.getTicket())) {
            logger.warn("RequestInfoHeader is missing or has placeholder ticket for /brief-voice-dnis. Authentication required.");
            return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
        }

        BriefInfoRequestType request = new BriefInfoRequestType(); // Assuming BriefInfoRequestType is appropriate
        request.setRequestInfoHeader(currentAuthHeader);

        ReturnMatrixDataTypeBG soapResponse;
        try {
            soapResponse = wsEMIS_port.briefVoiceDNIS(request);
        } catch (Exception e) {
            logger.error("Error calling briefVoiceDNIS SOAP operation", e);
            wsEMIS_port = null; // Consider resetting port on error
            return new ResponseEntity<>("Failed to fetch brief voice DNIS data: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse == null || soapResponse.getResponseInfoHeader() == null) {
            logger.error("Received null or invalid response from SOAP service for BriefVoiceDNIS");
            return new ResponseEntity<>("Invalid response from SOAP service for BriefVoiceDNIS", HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse.getResponseInfoHeader().getErrorCause() != 0) {
            logger.error("SOAP error in BriefVoiceDNIS response: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo());
            return new ResponseEntity<>("SOAP service error in BriefVoiceDNIS: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        BriefVoiceDnisResponseDto responseDto = new BriefVoiceDnisResponseDto();
        ResponseInfoHeaderDto responseHeaderDto = new ResponseInfoHeaderDto();
        responseHeaderDto.setServiceReqId(soapResponse.getResponseInfoHeader().getServiceReqId());
        responseHeaderDto.setErrorCause(soapResponse.getResponseInfoHeader().getErrorCause());
        responseHeaderDto.setServersInfo(soapResponse.getResponseInfoHeader().getServersInfo());
        responseDto.setResponseInfoHeader(responseHeaderDto);

        if (soapResponse.getReturnMatrix() != null) {
            List<DataItemRowDto> dataItemRows = new java.util.ArrayList<>();
            for (tadiran.emisweb.DataItemRow row : soapResponse.getReturnMatrix()) {
                DataItemRowDto rowDto = new DataItemRowDto();
                if (row.getReturnArray() != null) {
                    List<DataItemDto> dataItems = row.getReturnArray().stream()
                            .map(item -> {
                                DataItemDto itemDto = new DataItemDto();
                                itemDto.setErsid(item.getErsid());
                                itemDto.setValue(item.getValue());
                                itemDto.setErsname(item.getErsname());
                                itemDto.setBgcolor(item.getBgcolor() != null ? (long) item.getBgcolor() : null);
                                return itemDto;
                            })
                            .collect(Collectors.toList());
                    rowDto.setReturnArray(dataItems);
                } else {
                    rowDto.setReturnArray(java.util.Collections.emptyList());
                }
                dataItemRows.add(rowDto);
            }
            responseDto.setReturnMatrix(dataItemRows);
        } else {
            responseDto.setReturnMatrix(java.util.Collections.emptyList());
        }

        logger.info("Successfully fetched BriefVoiceDNIS. Returning " + (responseDto.getReturnMatrix() != null ? responseDto.getReturnMatrix().size() : 0) + " rows.");
        return ResponseEntity.ok(responseDto);
    }

    @PostMapping("/api/soap/save-report")
    public ResponseEntity<?> saveReport(@RequestBody SaveReportRequestDto saveReportRequestDto) {
        logger.info("Received request to save report: " + (saveReportRequestDto.getRequestItem() != null ? saveReportRequestDto.getRequestItem().getReportName() : "N/A"));

        if (wsEMIS_port == null) {
            try {
                wsEMIS_port = TemplateController.establishServerConnection();
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            } catch (MalformedURLException e) {
                logger.error("Error establishing server connection for Save Report", e);
                return new ResponseEntity<>("Internal server error: Could not connect to backend service.", HttpStatus.INTERNAL_SERVER_ERROR);
            }
        }

        RequestInfoHeaderType reqHeader;
        if (saveReportRequestDto.getRequestInfoHeader() != null) {
            reqHeader = new RequestInfoHeaderType();
            reqHeader.setServiceReqId(saveReportRequestDto.getRequestInfoHeader().getServiceReqId());
            reqHeader.setTicket(saveReportRequestDto.getRequestInfoHeader().getTicket());
            // Add other fields if necessary from DTO to SOAP type
        } else {
            // Fallback or error handling if requestInfoHeader is null in DTO
            logger.warn("RequestInfoHeaderDto is missing in the request for /save-report.");
            // Using placeholder, but ideally, this should be handled based on auth state
            reqHeader = getRequestInfoHeader(); // This will now use the stored header
            if (reqHeader == null || reqHeader.getTicket() == null || reqHeader.getTicket().isEmpty() || "placeholder_ticket".equals(reqHeader.getTicket())) {
                logger.warn("RequestInfoHeader is missing or has placeholder ticket for /save-report. Authentication required.");
                return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
            }
        }

        tadiran.emisweb.RequestSaveReportType soapRequest = new tadiran.emisweb.RequestSaveReportType();
        soapRequest.setRequestInfoHeader(reqHeader); // reqHeader is now correctly populated if DTO was null

        ReportListItemDto reportItemDto = saveReportRequestDto.getRequestItem();
        if (reportItemDto != null) {
            tadiran.emisweb.ReportListDataItemType soapReportItem = new tadiran.emisweb.ReportListDataItemType();
            soapReportItem.setReportId(reportItemDto.getReportId());
            soapReportItem.setReportName(reportItemDto.getReportName());
            soapReportItem.setRenderName(reportItemDto.getRenderName());
            soapReportItem.setParams(reportItemDto.getParams());
            soapReportItem.setIsPublic(reportItemDto.getIsPublic());
            soapRequest.setRequestItem(soapReportItem); // Set the nested RequestItem
        } else {
            logger.error("ReportListItemDto is missing in SaveReportRequestDto");
            return new ResponseEntity<>("Report details are missing in the request.", HttpStatus.BAD_REQUEST);
        }

        tadiran.emisweb.ResponseSaveReportType soapResponse; // Corrected type
        try {
            soapResponse = wsEMIS_port.saveReport(soapRequest);
        } catch (Exception e) {
            logger.error("Error calling saveReport SOAP operation", e);
            wsEMIS_port = null;
            return new ResponseEntity<>("Failed to save report: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse == null) { // Check if soapResponse itself is null
            logger.error("Received null response from SOAP service for SaveReport");
            return new ResponseEntity<>("Invalid response from SOAP service for SaveReport (null response object)", HttpStatus.INTERNAL_SERVER_ERROR);
        }

        SaveReportResponseDto responseDto = new SaveReportResponseDto();
        responseDto.setServiceReqId(soapResponse.getServiceReqId()); // Direct access
        responseDto.setErrorCause(soapResponse.getErrorCause());     // Direct access
        // serversInfo is not part of ResponseSaveReportType, so not attempting to map it.

        if (soapResponse.getErrorCause() != 0) { // Check direct errorCause
            logger.error("SOAP error in SaveReport response: " + soapResponse.getErrorCause());
            responseDto.setReportId(0); // Or some indicator of failure
            return new ResponseEntity<>(responseDto, HttpStatus.INTERNAL_SERVER_ERROR);
        }

        responseDto.setReportId(soapResponse.getReportId()); // Direct access

        logger.info("Successfully saved report. Report ID: " + soapResponse.getReportId());
        return ResponseEntity.ok(responseDto);
    }

    @GetMapping("/api/soap/email-dnis")
    public ResponseEntity<?> getEmailDnisList() {
        logger.info("Received request for /api/modernized/email-dnis");

        if (wsEMIS_port == null) {
            try {
                wsEMIS_port = TemplateController.establishServerConnection();
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            } catch (MalformedURLException e) {
                logger.error("Error establishing server connection for Email DNIS List", e);
                return new ResponseEntity<>("Internal server error: Could not connect to backend service.", HttpStatus.INTERNAL_SERVER_ERROR);
            }
        }

        RequestInfoHeaderType currentAuthHeader = getRequestInfoHeader();
        if (currentAuthHeader == null || currentAuthHeader.getTicket() == null || currentAuthHeader.getTicket().isEmpty() || "placeholder_ticket".equals(currentAuthHeader.getTicket())) {
            logger.warn("RequestInfoHeader is missing or has placeholder ticket for /email-dnis. Authentication required.");
            return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
        }

        GenRequestType request = new GenRequestType();
        request.setRequestInfoHeader(currentAuthHeader);

        DNISListReturnType soapResponse;
        try {
            soapResponse = wsEMIS_port.emailDNISList(request); // Calling emailDNISList
        } catch (Exception e) {
            logger.error("Error calling emailDNISList SOAP operation", e);
            wsEMIS_port = null; // Consider resetting port on error
            return new ResponseEntity<>("Failed to fetch email DNIS list: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse == null || soapResponse.getResponseInfoHeader() == null) {
            logger.error("Received null or invalid response from SOAP service for EmailDNISList");
            return new ResponseEntity<>("Invalid response from SOAP service for EmailDNISList", HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse.getResponseInfoHeader().getErrorCause() != 0) {
            logger.error("SOAP error in EmailDNISList response: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo());
            return new ResponseEntity<>("SOAP service error in EmailDNISList: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        DnisListResponseDto responseDto = new DnisListResponseDto();
        ResponseInfoHeaderDto responseHeaderDto = new ResponseInfoHeaderDto();
        responseHeaderDto.setServiceReqId(soapResponse.getResponseInfoHeader().getServiceReqId());
        responseHeaderDto.setErrorCause(soapResponse.getResponseInfoHeader().getErrorCause());
        responseHeaderDto.setServersInfo(soapResponse.getResponseInfoHeader().getServersInfo());
        responseDto.setResponseInfoHeader(responseHeaderDto);

        if (soapResponse.getReturnArray() != null) {
            List<DnisListItemDto> dnisListItems = soapResponse.getReturnArray().stream()
                    .map(item -> {
                        DnisListItemDto itemDto = new DnisListItemDto();
                        itemDto.setDnisId(item.getDnisId());
                        itemDto.setDnisName(item.getDnisName());
                        itemDto.setDnisNumber(item.getDnisNumber());
                        itemDto.setDnisType(item.getDnisType());
                        return itemDto;
                    })
                    .collect(Collectors.toList());
            responseDto.setReturnArray(dnisListItems);
        } else {
            responseDto.setReturnArray(java.util.Collections.emptyList());
        }

        logger.info("Successfully fetched EmailDNISList. Returning " + (responseDto.getReturnArray() != null ? responseDto.getReturnArray().size() : 0) + " items.");
        return ResponseEntity.ok(responseDto);
    }

    @PostMapping("/api/soap/delete-report")
    public ResponseEntity<?> deleteReport(@RequestBody DeleteReportRequestDto deleteReportRequestDto) {
        logger.info("Received request to delete report with ID: " + deleteReportRequestDto.getReportId());

        if (wsEMIS_port == null) {
            try {
                wsEMIS_port = TemplateController.establishServerConnection();
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            } catch (MalformedURLException e) {
                logger.error("Error establishing server connection for Delete Report", e);
                return new ResponseEntity<>("Internal server error: Could not connect to backend service.", HttpStatus.INTERNAL_SERVER_ERROR);
            }
        }

        RequestInfoHeaderType reqHeader;
        if (deleteReportRequestDto.getRequestInfoHeader() != null) {
            reqHeader = new RequestInfoHeaderType();
            reqHeader.setServiceReqId(deleteReportRequestDto.getRequestInfoHeader().getServiceReqId());
            reqHeader.setTicket(deleteReportRequestDto.getRequestInfoHeader().getTicket());
        } else {
            logger.warn("RequestInfoHeaderDto is missing in the request for /delete-report.");
            reqHeader = getRequestInfoHeader(); // Fallback, this will use the stored header
            if (reqHeader == null || reqHeader.getTicket() == null || reqHeader.getTicket().isEmpty() || "placeholder_ticket".equals(reqHeader.getTicket())) {
                logger.warn("RequestInfoHeader is missing or has placeholder ticket for /delete-report. Authentication required.");
                return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
            }
        }

        tadiran.emisweb.RequestDeleteReportType soapRequest = new tadiran.emisweb.RequestDeleteReportType();
        soapRequest.setRequestInfoHeader(reqHeader); // reqHeader is now correctly populated if DTO was null
        soapRequest.setReportId(deleteReportRequestDto.getReportId());

        ResponseInfoHeaderType soapResponse;
        try {
            soapResponse = wsEMIS_port.deleteReport(soapRequest);
        } catch (Exception e) {
            logger.error("Error calling deleteReport SOAP operation", e);
            wsEMIS_port = null;
            return new ResponseEntity<>("Failed to delete report: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse == null) {
            logger.error("Received null response from SOAP service for DeleteReport");
            return new ResponseEntity<>("Invalid response from SOAP service for DeleteReport", HttpStatus.INTERNAL_SERVER_ERROR);
        }

        ResponseInfoHeaderDto responseDto = new ResponseInfoHeaderDto();
        responseDto.setServiceReqId(soapResponse.getServiceReqId());
        responseDto.setErrorCause(soapResponse.getErrorCause());
        responseDto.setServersInfo(soapResponse.getServersInfo());

        if (soapResponse.getErrorCause() != 0) {
            logger.error("SOAP error in DeleteReport response: " + soapResponse.getErrorCause() + " - " + soapResponse.getServersInfo());
            return new ResponseEntity<>(responseDto, HttpStatus.INTERNAL_SERVER_ERROR);
        }

        logger.info("Successfully deleted report with ID: " + deleteReportRequestDto.getReportId());
        return ResponseEntity.ok(responseDto);
    }

    @GetMapping("/api/soap/brief-group")
    public ResponseEntity<?> getBriefGroupData() { // Renamed method and DTO
        logger.info("Received request for /api/soap/brief-group");

        if (wsEMIS_port == null) {
            try {
                wsEMIS_port = TemplateController.establishServerConnection();
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            } catch (MalformedURLException e) {
                logger.error("Error establishing server connection for Brief Group Data", e);
                return new ResponseEntity<>("Internal server error: Could not connect to backend service.", HttpStatus.INTERNAL_SERVER_ERROR);
            }
        }

        RequestInfoHeaderType currentAuthHeader = getRequestInfoHeader();
        if (currentAuthHeader == null || currentAuthHeader.getTicket() == null || currentAuthHeader.getTicket().isEmpty() || "placeholder_ticket".equals(currentAuthHeader.getTicket())) {
            logger.warn("RequestInfoHeader is missing or has placeholder ticket for /brief-group. Authentication required.");
            return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
        }

        GenInfoRequestType request = new GenInfoRequestType(); // Changed to GenInfoRequestType
        request.setRequestInfoHeader(currentAuthHeader);
        // If GroupId is needed, it should be passed as a parameter, e.g., @PathVariable or @RequestParam
        // request.setGroupId(...); // Assuming GenInfoRequestType might have a method like setObjectId or similar

        ReturnArrayDataType soapResponse;
        try {
            soapResponse = wsEMIS_port.groupDailyBriefInfo(request); // Use GenInfoRequestType
        } catch (Exception e) {
            logger.error("Error calling groupDailyBriefInfo SOAP operation", e);
            wsEMIS_port = null;
            return new ResponseEntity<>("Failed to fetch brief group data: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse == null || soapResponse.getResponseInfoHeader() == null) {
            logger.error("Received null or invalid response from SOAP service for GroupDailyBriefInfo");
            return new ResponseEntity<>("Invalid response from SOAP service for GroupDailyBriefInfo", HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse.getResponseInfoHeader().getErrorCause() != 0) {
            logger.error("SOAP error in GroupDailyBriefInfo response: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo());
            return new ResponseEntity<>("SOAP service error in GroupDailyBriefInfo: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        BriefGroupResponseDto responseDto = new BriefGroupResponseDto();
        ResponseInfoHeaderDto responseHeaderDto = new ResponseInfoHeaderDto();
        responseHeaderDto.setServiceReqId(soapResponse.getResponseInfoHeader().getServiceReqId());
        responseHeaderDto.setErrorCause(soapResponse.getResponseInfoHeader().getErrorCause());
        responseHeaderDto.setServersInfo(soapResponse.getResponseInfoHeader().getServersInfo());
        responseDto.setResponseInfoHeader(responseHeaderDto);

        if (soapResponse.getReturnArrayOfBlocks() != null) { // Assuming getReturnArrayOfBlocks() returns List<BlockItemType>
            List<BlockItemDto> blockItems = soapResponse.getReturnArrayOfBlocks().stream() // Removed .getBlockItem()
                    .map(soapBlock -> {
                        BlockItemDto blockDto = new BlockItemDto();
                        blockDto.setId(soapBlock.getId());
                        blockDto.setName(soapBlock.getName());
                        if (soapBlock.getReturnArray() != null) {
                            List<DataItemDto> dataItems = soapBlock.getReturnArray().stream()
                                    .map(soapDataItem -> {
                                        DataItemDto dataItemDto = new DataItemDto();
                                        dataItemDto.setErsid(soapDataItem.getErsid());
                                        dataItemDto.setValue(soapDataItem.getValue());
                                        dataItemDto.setErsname(soapDataItem.getErsname());
                                        dataItemDto.setBgcolor(soapDataItem.getBgcolor() != null ? soapDataItem.getBgcolor().longValue() : null);
                                        return dataItemDto;
                                    })
                                    .collect(Collectors.toList());
                            blockDto.setReturnArray(dataItems);
                        } else {
                            blockDto.setReturnArray(java.util.Collections.emptyList());
                        }
                        return blockDto;
                    })
                    .collect(Collectors.toList());
            responseDto.setReturnArrayOfBlocks(blockItems);
        } else {
            responseDto.setReturnArrayOfBlocks(java.util.Collections.emptyList());
        }

        logger.info("Successfully fetched GroupDailyBriefInfo. Returning " + (responseDto.getReturnArrayOfBlocks() != null ? responseDto.getReturnArrayOfBlocks().size() : 0) + " blocks.");
        return ResponseEntity.ok(responseDto);
    }

    @GetMapping("/api/soap/brief-super-group")
    public ResponseEntity<?> getBriefSuperGroupData() { // Renamed method and DTO
        logger.info("Received request for /api/soap/brief-super-group");

        if (wsEMIS_port == null) {
            try {
                wsEMIS_port = TemplateController.establishServerConnection();
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            } catch (MalformedURLException e) {
                logger.error("Error establishing server connection for Brief Super Group Data", e);
                return new ResponseEntity<>("Internal server error: Could not connect to backend service.", HttpStatus.INTERNAL_SERVER_ERROR);
            }
        }

        RequestInfoHeaderType currentAuthHeader = getRequestInfoHeader();
        if (currentAuthHeader == null || currentAuthHeader.getTicket() == null || currentAuthHeader.getTicket().isEmpty() || "placeholder_ticket".equals(currentAuthHeader.getTicket())) {
            logger.warn("RequestInfoHeader is missing or has placeholder ticket for /brief-super-group. Authentication required.");
            return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
        }

        GenInfoRequestType request = new GenInfoRequestType(); // Changed to GenInfoRequestType
        request.setRequestInfoHeader(currentAuthHeader);
        // If SuperGroupId is needed, it should be passed as a parameter
        // request.setSuperGroupId(...); // Assuming GenInfoRequestType might have a method like setObjectId or similar

        ReturnArrayDataType soapResponse;
        try {
            soapResponse = wsEMIS_port.superGroupDailyBriefInfo(request); // Use GenInfoRequestType
        } catch (Exception e) {
            logger.error("Error calling superGroupDailyBriefInfo SOAP operation", e);
            wsEMIS_port = null;
            return new ResponseEntity<>("Failed to fetch brief super group data: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse == null || soapResponse.getResponseInfoHeader() == null) {
            logger.error("Received null or invalid response from SOAP service for SuperGroupDailyBriefInfo");
            return new ResponseEntity<>("Invalid response from SOAP service for SuperGroupDailyBriefInfo", HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse.getResponseInfoHeader().getErrorCause() != 0) {
            logger.error("SOAP error in SuperGroupDailyBriefInfo response: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo());
            return new ResponseEntity<>("SOAP service error in SuperGroupDailyBriefInfo: " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        BriefSuperGroupResponseDto responseDto = new BriefSuperGroupResponseDto();
        ResponseInfoHeaderDto responseHeaderDto = new ResponseInfoHeaderDto();
        responseHeaderDto.setServiceReqId(soapResponse.getResponseInfoHeader().getServiceReqId());
        responseHeaderDto.setErrorCause(soapResponse.getResponseInfoHeader().getErrorCause());
        responseHeaderDto.setServersInfo(soapResponse.getResponseInfoHeader().getServersInfo());
        responseDto.setResponseInfoHeader(responseHeaderDto);

        if (soapResponse.getReturnArrayOfBlocks() != null) { // Assuming getReturnArrayOfBlocks() returns List<BlockItemType>
            List<BlockItemDto> blockItems = soapResponse.getReturnArrayOfBlocks().stream() // Removed .getBlockItem()
                    .map(soapBlock -> {
                        BlockItemDto blockDto = new BlockItemDto();
                        blockDto.setId(soapBlock.getId());
                        blockDto.setName(soapBlock.getName());
                        if (soapBlock.getReturnArray() != null) {
                            List<DataItemDto> dataItems = soapBlock.getReturnArray().stream()
                                    .map(soapDataItem -> {
                                        DataItemDto dataItemDto = new DataItemDto();
                                        dataItemDto.setErsid(soapDataItem.getErsid());
                                        dataItemDto.setValue(soapDataItem.getValue());
                                        dataItemDto.setErsname(soapDataItem.getErsname());
                                        dataItemDto.setBgcolor(soapDataItem.getBgcolor() != null ? soapDataItem.getBgcolor().longValue() : null);
                                        return dataItemDto;
                                    })
                                    .collect(Collectors.toList());
                            blockDto.setReturnArray(dataItems);
                        } else {
                            blockDto.setReturnArray(java.util.Collections.emptyList());
                        }
                        return blockDto;
                    })
                    .collect(Collectors.toList());
            responseDto.setReturnArrayOfBlocks(blockItems);
        } else {
            responseDto.setReturnArrayOfBlocks(java.util.Collections.emptyList());
        }

        logger.info("Successfully fetched SuperGroupDailyBriefInfo. Returning " + (responseDto.getReturnArrayOfBlocks() != null ? responseDto.getReturnArrayOfBlocks().size() : 0) + " blocks.");
        return ResponseEntity.ok(responseDto);
    }

    private ResponseEntity<?> getDetailInfo(String operationName, Integer id, DetailInfoFunction detailFunction) {
        logger.info("Received request for /api/soap/{}/{}", operationName, id);

        if (wsEMIS_port == null) {
            try {
                wsEMIS_port = TemplateController.establishServerConnection();
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            } catch (MalformedURLException e) {
                logger.error("Error establishing server connection for {}", operationName, e);
                return new ResponseEntity<>("Internal server error: Could not connect to backend service.", HttpStatus.INTERNAL_SERVER_ERROR);
            }
        }

        RequestInfoHeaderType currentAuthHeader = getRequestInfoHeader();
        if (currentAuthHeader == null || currentAuthHeader.getTicket() == null || currentAuthHeader.getTicket().isEmpty() || "placeholder_ticket".equals(currentAuthHeader.getTicket())) {
            logger.warn("RequestInfoHeader is missing or has placeholder ticket for /{}/{}. Authentication required.", operationName, id);
            return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
        }

        GenInfoRequestType request = new GenInfoRequestType();
        request.setRequestInfoHeader(currentAuthHeader);
        request.getId().add(id.longValue()); // Convert Integer to Long for the list

        ReturnArrayDataType soapResponse;
        try {
            soapResponse = detailFunction.apply(request);
        } catch (Exception e) {
            logger.error("Error calling {} SOAP operation for ID {}", operationName, id, e);
            wsEMIS_port = null; // Consider resetting port on error
            return new ResponseEntity<>("Failed to fetch " + operationName + " data: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse == null || soapResponse.getResponseInfoHeader() == null) {
            logger.error("Received null or invalid response from SOAP service for {} with ID {}", operationName, id);
            return new ResponseEntity<>("Invalid response from SOAP service for " + operationName, HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse.getResponseInfoHeader().getErrorCause() != 0) {
            logger.error("SOAP error in {} response for ID {}: {} - {}", operationName, id, soapResponse.getResponseInfoHeader().getErrorCause(), soapResponse.getResponseInfoHeader().getServersInfo());
            return new ResponseEntity<>("SOAP service error in " + operationName + ": " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        DetailResponseDto responseDto = new DetailResponseDto();
        ResponseInfoHeaderDto responseHeaderDto = new ResponseInfoHeaderDto();
        responseHeaderDto.setServiceReqId(soapResponse.getResponseInfoHeader().getServiceReqId());
        responseHeaderDto.setErrorCause(soapResponse.getResponseInfoHeader().getErrorCause());
        responseHeaderDto.setServersInfo(soapResponse.getResponseInfoHeader().getServersInfo());
        responseDto.setResponseInfoHeader(responseHeaderDto);

        if (soapResponse.getReturnArrayOfBlocks() != null) {
            List<BlockItemDto> blockItems = soapResponse.getReturnArrayOfBlocks().stream()
                    .map(soapBlock -> {
                        BlockItemDto blockDto = new BlockItemDto();
                        blockDto.setId(soapBlock.getId());
                        blockDto.setName(soapBlock.getName());
                        if (soapBlock.getReturnArray() != null) {
                            List<DataItemDto> dataItems = soapBlock.getReturnArray().stream()
                                    .map(soapDataItem -> {
                                        DataItemDto dataItemDto = new DataItemDto();
                                        dataItemDto.setErsid(soapDataItem.getErsid());
                                        dataItemDto.setValue(soapDataItem.getValue());
                                        dataItemDto.setErsname(soapDataItem.getErsname());
                                        dataItemDto.setBgcolor(soapDataItem.getBgcolor() != null ? soapDataItem.getBgcolor().longValue() : null);
                                        return dataItemDto;
                                    })
                                    .collect(Collectors.toList());
                            blockDto.setReturnArray(dataItems);
                        } else {
                            blockDto.setReturnArray(java.util.Collections.emptyList());
                        }
                        return blockDto;
                    })
                    .collect(Collectors.toList());
            responseDto.setReturnArrayOfBlocks(blockItems);
        } else {
            responseDto.setReturnArrayOfBlocks(java.util.Collections.emptyList());
        }

        logger.info("Successfully fetched {}. Returning {} blocks.", operationName, (responseDto.getReturnArrayOfBlocks() != null ? responseDto.getReturnArrayOfBlocks().size() : 0));
        return ResponseEntity.ok(responseDto);
    }

    @FunctionalInterface
    private interface DetailInfoFunction {
        ReturnArrayDataType apply(GenInfoRequestType request) throws Exception;
    }

    @GetMapping("/api/soap/group/{id}/detail-full")
    public ResponseEntity<?> getGroupDetailFullInfo(@PathVariable Integer id) {
        return getDetailInfo("groupDetailFullInfo", id, wsEMIS_port::groupDetailFullInfo);
    }

    @GetMapping("/api/soap/group/{id}/queue")
    public ResponseEntity<?> getGroupQueInfo(@PathVariable Integer id) {
        return getDetailInfo("groupQueInfo", id, wsEMIS_port::groupQueInfo);
    }

    @GetMapping("/api/soap/group/{id}/agents-info")
    public ResponseEntity<?> getGroupAgentsInfo(@PathVariable Integer id) {
        return getDetailInfo("groupAgentsInfo", id, wsEMIS_port::groupAgentsInfo);
    }

    @GetMapping("/api/soap/ivr/application/{id}")
    public ResponseEntity<?> getIvrApplicationInfo(@PathVariable Integer id) {
        return getDetailInfo("ivrApplInfo", id, wsEMIS_port::ivrApplInfo);
    }

    @GetMapping("/api/soap/ivr/port/{id}")
    public ResponseEntity<?> getIvrPortInfo(@PathVariable Integer id) {
        return getDetailInfo("ivrPortInfo", id, wsEMIS_port::ivrPortInfo);
    }

    @GetMapping("/api/soap/group/{id}/abandoned-info")
    public ResponseEntity<?> getGroupAbandonedInfo(@PathVariable Integer id) {
        return getDetailInfo("groupAbndInfo", id, wsEMIS_port::groupAbndInfo);
    }

    @GetMapping("/api/soap/group/{id}/internal-overflow-info")
    public ResponseEntity<?> getGroupInternalOverflowInfo(@PathVariable Integer id) {
        return getDetailInfo("groupIntOvfInfo", id, wsEMIS_port::groupIntOvfInfo);
    }

    @GetMapping("/api/soap/group/{id}/shift-info")
    public ResponseEntity<?> getGroupShiftInfo(@PathVariable Integer id) {
        return getDetailInfo("groupShiftInfo", id, wsEMIS_port::groupShiftInfo);
    }

    @GetMapping("/api/soap/group/{id}/general-info")
    public ResponseEntity<?> getGroupGeneralInfo(@PathVariable Integer id) {
        return getDetailInfo("groupGeneralInfo", id, wsEMIS_port::groupGeneralInfo);
    }

    @GetMapping("/api/soap/group/{id}/period-info")
    public ResponseEntity<?> getGroupPeriodInfo(@PathVariable Integer id) {
        return getDetailInfo("groupPeriodInfo", id, wsEMIS_port::groupPeriodInfo);
    }

    @GetMapping("/api/soap/ivr/group/{id}")
    public ResponseEntity<?> getIvrGroupInfo(@PathVariable Integer id) {
        return getDetailInfo("ivrGrpInfo", id, wsEMIS_port::ivrGrpInfo);
    }

    @GetMapping("/api/soap/mailbox/{id}")
    public ResponseEntity<?> getMailboxInfo(@PathVariable Integer id) {
        return getDetailInfo("mailboxInfo", id, wsEMIS_port::mailboxInfo);
    }

    @GetMapping("/api/soap/super-group/{id}/info")
    public ResponseEntity<?> getSuperGroupDetailInfo(@PathVariable Integer id) {
        return getDetailInfo("superGroupInfo", id, wsEMIS_port::superGroupInfo);
    }

    @GetMapping("/api/soap/group/{id}/daily-detailed")
    public ResponseEntity<?> getGroupDailyDetailedInfo(@PathVariable Integer id) {
        return getDetailInfo("groupDailyDetailedInfo", id, wsEMIS_port::groupDailyDetailedInfo);
    }

    // Helper for GenListReturnType endpoints
    @FunctionalInterface
    private interface GenericListFunction {
        GenListReturnType apply(GenRequestType request) throws Exception;
    }

    private ResponseEntity<?> getGenericList(String operationName, GenericListFunction listFunction) {
        logger.info("Received request for /api/modernized/{}", operationName);

        if (wsEMIS_port == null) {
            try {
                wsEMIS_port = TemplateController.establishServerConnection();
                if (wsEMIS_port == null) {
                    return new ResponseEntity<>("Service connection error: Main service not available.", HttpStatus.SERVICE_UNAVAILABLE);
                }
            } catch (MalformedURLException e) {
                logger.error("Error establishing server connection for {}", operationName, e);
                return new ResponseEntity<>("Internal server error: Could not connect to backend service.", HttpStatus.INTERNAL_SERVER_ERROR);
            }
        }

        RequestInfoHeaderType currentAuthHeader = getRequestInfoHeader();
        if (currentAuthHeader == null || currentAuthHeader.getTicket() == null || currentAuthHeader.getTicket().isEmpty() || "placeholder_ticket".equals(currentAuthHeader.getTicket())) {
            logger.warn("RequestInfoHeader is missing or has placeholder ticket for /{}. Authentication required.", operationName);
            return new ResponseEntity<>("User not authenticated or session expired. Please login again.", HttpStatus.UNAUTHORIZED);
        }

        GenRequestType request = new GenRequestType();
        request.setRequestInfoHeader(currentAuthHeader);

        GenListReturnType soapResponse;
        try {
            soapResponse = listFunction.apply(request);
        } catch (Exception e) {
            logger.error("Error calling {} SOAP operation", operationName, e);
            wsEMIS_port = null; // Consider resetting port on error
            return new ResponseEntity<>("Failed to fetch " + operationName + " data: " + e.getMessage(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse == null || soapResponse.getResponseInfoHeader() == null) {
            logger.error("Received null or invalid response from SOAP service for {}", operationName);
            return new ResponseEntity<>("Invalid response from SOAP service for " + operationName, HttpStatus.INTERNAL_SERVER_ERROR);
        }

        if (soapResponse.getResponseInfoHeader().getErrorCause() != 0) {
            logger.error("SOAP error in {} response: {} - {}", operationName, soapResponse.getResponseInfoHeader().getErrorCause(), soapResponse.getResponseInfoHeader().getServersInfo());
            return new ResponseEntity<>("SOAP service error in " + operationName + ": " + soapResponse.getResponseInfoHeader().getErrorCause() + " - " + soapResponse.getResponseInfoHeader().getServersInfo(), HttpStatus.INTERNAL_SERVER_ERROR);
        }

        GenListResponseDto responseDto = new GenListResponseDto();
        ResponseInfoHeaderDto responseHeaderDto = new ResponseInfoHeaderDto();
        responseHeaderDto.setServiceReqId(soapResponse.getResponseInfoHeader().getServiceReqId());
        responseHeaderDto.setErrorCause(soapResponse.getResponseInfoHeader().getErrorCause());
        responseHeaderDto.setServersInfo(soapResponse.getResponseInfoHeader().getServersInfo());
        responseDto.setResponseInfoHeader(responseHeaderDto);

        if (soapResponse.getReturnArray() != null) {
            List<GenListItemDto> listItems = soapResponse.getReturnArray().stream()
                    .map(item -> {
                        GenListItemDto itemDto = new GenListItemDto();
                        itemDto.setId(item.getId());
                        itemDto.setName(item.getName());
                        return itemDto;
                    })
                    .collect(Collectors.toList());
            responseDto.setReturnArray(listItems);
        } else {
            responseDto.setReturnArray(java.util.Collections.emptyList());
        }

        logger.info("Successfully fetched {}. Returning {} items.", operationName, (responseDto.getReturnArray() != null ? responseDto.getReturnArray().size() : 0));
        return ResponseEntity.ok(responseDto);
    }

    @GetMapping("/api/soap/ivr/port-groups")
    public ResponseEntity<?> getIvrPortGroupList() {
        return getGenericList("ivrPortGroupList", wsEMIS_port::ivrPortGroupList);
    }

    @GetMapping("/api/soap/ivr/applications")
    public ResponseEntity<?> getIvrApplicationList() {
        return getGenericList("ivrAppList", wsEMIS_port::ivrAppList);
    }

    @GetMapping("/api/soap/rcodes")
    public ResponseEntity<?> getRCodeList() {
        return getGenericList("rCodeList", wsEMIS_port::rCodeList);
    }

    @GetMapping("/api/soap/mail-accounts")
    public ResponseEntity<?> getMailAccountList() {
        return getGenericList("mailAccList", wsEMIS_port::mailAccList);
    }

}
