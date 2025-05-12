package tadiran.webnla.accdashboard.config;

import org.apache.logging.log4j.LogManager;
import org.springframework.context.ApplicationContextInitializer;
import org.springframework.context.ConfigurableApplicationContext;
import org.springframework.core.env.ConfigurableEnvironment;
import org.springframework.core.io.support.ResourcePropertySource;

import java.io.IOException;

public class AdminApplicationContextInitializer implements ApplicationContextInitializer<ConfigurableApplicationContext> {
    private static final org.apache.logging.log4j.Logger logger = LogManager.getLogger(AdminApplicationContextInitializer.class);


    @Override
    public void initialize(ConfigurableApplicationContext applicationContext) {

        ConfigurableEnvironment environment = applicationContext.getEnvironment();

        environment.setActiveProfiles("profileName");
        try {
            environment.getPropertySources().addFirst(new ResourcePropertySource("classpath:application.properties"));
            logger.info("tadiran.dashboard.ACCServerPort1: " + environment.getProperty("tadiran.dashboard.ACCServerPort1"));


            //PropertiesManager.setEnvironment(environment);
            logger.info("env.properties loaded");
        } catch (IOException e) {
            // it's ok if the file is not there. we will just log that info.
            logger.info("didn't find env.properties in classpath so not loading it in the AppContextInitialized");
        }
    }

}
