package tadiran.webnla.config;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.boot.ApplicationRunner;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.env.Environment;
import org.springframework.core.io.ClassPathResource;
import tadiran.webnla.model.Prop;

import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Properties;


@Configuration
public class PropertiesManager {
    private static final Logger logger = LogManager.getLogger(PropertiesManager.class);

    static Environment environment;
    public Environment getEnvironment() {return environment;}
    public static void setEnvironment(Environment environment) {PropertiesManager.environment = environment;}

    //static ConfigurableEnvironment environment;
    //public ConfigurableEnvironment getEnvironment() {return environment;}
    //public static void setEnvironment(ConfigurableEnvironment environment) {PropertiesManager.environment = environment;}

    private final Properties properties;

    private final String backUpPropertiesFile = "application.properties";
    private final String defaultPropertiesFile = "/WEB-INF/application.properties";

    public PropertiesManager() {
        properties = new Properties();
    }

    public static Object getProperty(String key, Class<?> propClass) {
        return environment.getProperty(key, propClass);
    }

    public void setProperty(String key, String val) {
        properties.setProperty(key, val);
    }

    public void printProperties() {


        logger.info("tadiran.dashboard.ACCServerAddress1: " + ((String) PropertiesManager.getProperty("tadiran.dashboard.ACCServerAddress1", String.class)));
        logger.info("tadiran.dashboard.ACCServerAddress2: " + ((String) PropertiesManager.getProperty("tadiran.dashboard.ACCServerAddress2", String.class)));
        logger.info("tadiran.dashboard.ACCServerPort1: " + ((Integer) PropertiesManager.getProperty("tadiran.dashboard.ACCServerPort1", Integer.class)));
        logger.info("tadiran.dashboard.ACCServerPort2: " + ((String) PropertiesManager.getProperty("tadiran.dashboard.ACCServerPort2", String.class)));
        logger.info("tadiran.dashboard.ACCWEBServers: " + ((String) PropertiesManager.getProperty("tadiran.dashboard.ACCWEBServers", String.class)));

    }

    public List<Prop> getPreferences() {
        FileReader reader = null;
        Properties preferences = new Properties();

        try {
            File file = new ClassPathResource(backUpPropertiesFile).getFile();
            reader = new FileReader(file);
            preferences.load(reader);
        } catch (IOException e) {
            e.printStackTrace();
        }

        List<Prop> propList = new ArrayList<>();
        for (Map.Entry<Object, Object> p  : preferences.entrySet()) {
            propList.add(new Prop((String) p.getKey(),(String) p.getValue()));
        }

        logger.info("getPreferences(): Preferences read successfully.");
        return propList;
    }

    public void applyPreferences() {
        Properties preferences = new Properties();

        try {
            FileReader reader = new FileReader( new ClassPathResource(backUpPropertiesFile).getFile());
            preferences.load(reader);
        } catch (IOException e) {
            e.printStackTrace();
        }

        for (Map.Entry<Object, Object> p  : preferences.entrySet()) {
            System.setProperty((String) p.getKey(), (String) p.getValue());
        }

        logger.info("applyPreferences(): Preferences apply successfully.");
        printProperties();
    }

    public boolean savePreferences(List<Prop>  prop) {
        Properties preferences = new Properties();

        for (Prop p : prop)
            preferences.setProperty(p.getPropName(), p.getPropValue());


        try {
            FileWriter writer = new FileWriter( new ClassPathResource(backUpPropertiesFile).getFile());
            preferences.store(writer, "Aeonix-App-Center preferences.");
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        logger.info("savePreferences(): Preferences saved successfully.");
        this.applyPreferences();
        return true;
    }

    public boolean resetPreferences() {
        List<Prop> prefList = this.getPreferences();
        Properties properties = new Properties();

        try {
            // Read the default values from the file 'application.properties'.
            FileReader reader = new FileReader(new ClassPathResource(defaultPropertiesFile).getFile());
            properties.load(reader);

            // Update the list of preferences (prefList) with the original value from 'application.properties' (properties).
            for (Prop p  : prefList)
                p.setPropValue(properties.getProperty(p.getPropName()));

            // Hard-coded the resorting of the original preferences to a file 'saved-preference.properties'.
            this.savePreferences(prefList);

            // Apply the new values on the current run.
            this.applyPreferences();


        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }



        return true;
    }

    @Bean(name = "PropertiesManager")
    ApplicationRunner applicationRunner(Environment environment) {
        PropertiesManager.environment = environment;
        this.applyPreferences();

        return args -> {
            logger.info("tadiran.dashboard.ACCServerAddress1: " + environment.getProperty("tadiran.dashboard.ACCServerAddress1"));
            logger.info("tadiran.dashboard.ACCServerAddress2: " + environment.getProperty("tadiran.dashboard.ACCServerAddress2"));
            logger.info("tadiran.dashboard.ACCServerPort1: " + environment.getProperty("tadiran.dashboard.ACCServerPort1"));
            logger.info("tadiran.dashboard.ACCServerPort2: " + environment.getProperty("tadiran.dashboard.ACCServerPort2"));
            logger.info("tadiran.dashboard.ACCWEBServers: " + environment.getProperty("tadiran.dashboard.ACCWEBServers"));
        };
    }
}
