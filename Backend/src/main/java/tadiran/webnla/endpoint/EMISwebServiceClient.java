package tadiran.webnla.endpoint;

import tadiran.emisweb.EMISwebService;

import javax.xml.namespace.QName;
import java.net.MalformedURLException;
import java.net.URL;


/**
 *
 * @author genvab
 */
public class EMISwebServiceClient extends EMISwebService {
    private final static QName EMISWEBSERVICE_QNAME = new QName("urn:EMISweb", "EMISwebService");

    
    public EMISwebServiceClient(String url, String port) throws MalformedURLException {
        super(new URL("http://"+url+":"+port+"?wsdl"), EMISWEBSERVICE_QNAME);
    }
}
