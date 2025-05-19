package tadiran.webnla.config;

import org.springframework.boot.web.servlet.ServletRegistrationBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import tadiran.webnla.servlet.NLAService;

@Configuration
public class WebAppConfig {

    @Bean
    public ServletRegistrationBean<NLAService> nlaServiceRegistrationBean() {
        // The URL pattern from web.xml was /nlatRescan
        ServletRegistrationBean<NLAService> registrationBean = 
            new ServletRegistrationBean<>(new NLAService(), "/nlatRescan");
        // The servlet-name from web.xml was NLAStart
        registrationBean.setName("NLAStart");
        // The load-on-startup value from web.xml was 5
        registrationBean.setLoadOnStartup(5); 
        return registrationBean;
    }
}
