package tadiran.webnla.filter;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import jakarta.servlet.Filter;
import jakarta.servlet.FilterChain;
import jakarta.servlet.FilterConfig;
import jakarta.servlet.ServletException;
import jakarta.servlet.ServletRequest;
import jakarta.servlet.ServletResponse;
import jakarta.servlet.http.Cookie;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

import io.jsonwebtoken.*;
import java.io.PrintWriter;
import java.util.Collection;
import java.util.Locale;
import jakarta.servlet.ServletOutputStream;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import static tadiran.webnla.filter.AuthTokenFilter.LOGGER;
import tadiran.webnla.utils.WStrUtils;
//import org.netbeans.lib.profiler.utils.StringUtils;


public class AuthTokenFilter implements Filter {

    final static Logger LOGGER = LogManager.getLogger("AuthTokenFilter");

    /** directories which should not be authenticated irrespective of filter-mapping. */
    private final transient List<String> excludeDirs = new ArrayList<String>();

    /**
     * Default constructor.
     */
    public AuthTokenFilter() {
        // TODO Auto-generated constructor stub
        LOGGER.info("constructor");
    }


    @Override
    public void destroy() {
            LOGGER.info("destroy AuthTokenFilter");

            if (null != this.excludeDirs) {
                    this.excludeDirs.clear();
            }

        // TODO Auto-generated method stub
    }


    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain chain)
            throws IOException, ServletException {

            final HttpServletRequest request = (HttpServletRequest) servletRequest;

            // Authorize (allow) all domains to consume the content
            ((HttpServletResponse) servletResponse).setHeader("Access-Control-Allow-Origin", "*");
            ((HttpServletResponse) servletResponse).setHeader("Access-Control-Allow-Headers", "content-type,Authorization");
            ((HttpServletResponse) servletResponse).setHeader("Access-Control-Allow-Methods","GET, OPTIONS, HEAD, PUT, POST");


            final HttpServletResponse response = (HttpServletResponse) servletResponse;

            LOGGER.debug("JWT token:    -" + request.getRequestURI() + "-");


            boolean valid = false;
            String username = "";
            try {
                    String jwt = parseJwt(request);
                    if (!WStrUtils.isEmptyStr(jwt) && validateJwtToken(jwt)) {
                            username = getUserNameFromJwtToken(jwt);
                            valid = true;
                    }
            } catch (Exception e) {
                    LOGGER.error("Cannot set user authentication: {}", e);
            }

            

            if(     /* request.getRequestURI().startsWith("/accDashboard/") || */
                    request.getRequestURI().endsWith("/accDashboard/login") ||
                    request.getRequestURI().endsWith("/accDashboard/login.jsp") ||
                    request.getRequestURI().startsWith("/nlat/**") ||
                    request.getRequestURI().endsWith("/accDashboard/") ||
                    request.getRequestURI().startsWith("/accDashboard/js/tableMng.js") ||
                    request.getRequestURI().matches("\\/accDashboard\\/(?:title|main|index)\\.jsp$") ||
                    request.getRequestURI().endsWith("/accDashboard/") ||
                    request.getRequestURI().endsWith(".jsp") ||
                    request.getRequestURI().endsWith(".png") ||
                    request.getRequestURI().endsWith(".ico") ||
                    request.getRequestURI().endsWith(".gif") ||
                    request.getRequestURI().endsWith(".css")    ){
                    LOGGER.debug("JWT token permited by url" + username);
                    valid =true;
                    //chain.doFilter(request, response);
                    //return;
            }

            
            // For HTTP OPTIONS verb/method reply with ACCEPTED status code -- per CORS handshake
            if (!valid) {
                    LOGGER.info("JWT token not valid ");
                    StringBuilder sb = new StringBuilder();
                    sb.append("{ ");
                    sb.append("\"error\": \"Unauthorized\",");
                    sb.append("\"message\": \"Invalid Token.\",");  //<--- your message here
                    sb.append("\"path\": \"")
                      .append(request.getRequestURL())
                      .append("\"");
                    sb.append("} ");

                    response.setContentType("application/json");
                    response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
                    response.setContentLength(sb.length());
                    response.getWriter().write(sb.toString());
                    //return;
                    
                    
                    /*response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
                    response.sendRedirect("/accDashboard/index.jsp");
                    return;*/
            }else {
                    //LOGGER.debug("the JWT user from the token: " + username);
            }
            
            
        //LOGGER.debug("JWT token permited by token" + username);    
        // pass the request along the filter chain
        chain.doFilter(request, response);
    }


    @Override
    public void init(FilterConfig fConfig) throws ServletException {
            LOGGER.info("init AuthTokenFilter: " + fConfig.toString());

        // TODO Auto-generated method stub
    }

    private String parseJwt(HttpServletRequest request) {
        
            String headerAuth = request.getHeader("Authorization");
            if (!WStrUtils.isEmptyStr(headerAuth) && headerAuth.startsWith("Bearer ")) {
                    //LOGGER.debug("JWT token: found token in Header!" + "  " + headerAuth.substring(7, headerAuth.length()));
                    return headerAuth.substring(7, headerAuth.length());
            }
        
            
            
            Cookie[] cookies = request.getCookies();
            if(cookies != null){
                    for(Cookie cookie : cookies){
                        if(cookie.getName().equals("Authorization")) {
                            //LOGGER.debug("JWT token: found token in Cookie!" + "  " + cookie.getValue());
                            headerAuth = cookie.getValue().toString();
                            return headerAuth;
                        }
                    }
            }
        
            return null;
    }

    public static String getUserNameFromJwtToken(String token) {
            return Jwts.parser().setSigningKey(jwtSecret).parseClaimsJws(token).getBody().getSubject();
    }

    public boolean validateJwtToken(String authToken) {
            try {
                    Jwts.parser().setSigningKey(jwtSecret).parseClaimsJws(authToken);
                    return true;
            } catch (SignatureException e) {
                    LOGGER.error("Invalid JWT signature: {}", e);
            } catch (MalformedJwtException e) {
                    LOGGER.error("Invalid JWT token: {}", e);
            } catch (ExpiredJwtException e) {
                    LOGGER.error("JWT token is expired: {}", e);
            } catch (UnsupportedJwtException e) {
                    LOGGER.error("JWT token is unsupported: {}", e);
            } catch (IllegalArgumentException e) {
                    LOGGER.error("JWT claims string is empty: {}", e);
            }

            return false;
    }

    //@Value("${bezkoder.app.jwtSecret}") //@Value("${bezkoder.app.jwtSecret}")
    private static String jwtSecret = "e1996-secret-key";

    
}
