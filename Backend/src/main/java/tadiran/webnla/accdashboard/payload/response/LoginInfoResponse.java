package tadiran.webnla.accdashboard.payload.response;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Getter
@Setter
public class LoginInfoResponse {
    private long id;
    private String username;
    private String agentNo;
    private List<String> roles;
    private String sessionId;
    private String extension;
    private String type = "Bearer";




    public LoginInfoResponse(long id, String username, String agentNo, List<String> roles, String sessionId, String extension) {
        this.id = id;
        this.username = username;
        this.agentNo = agentNo;
        this.roles = roles;
        this.sessionId = sessionId;
        this.extension = extension;
    }

    public LoginInfoResponse(long id, String username, String agentNo, List<String> roles, String sessionId) {
        this.id = id;
        this.username = username;
        this.agentNo = agentNo;
        this.roles = roles;
        this.sessionId = sessionId;
    }

    public LoginInfoResponse(long id, String username, List<String> roles, String sessionId) {
        this.id = id;
        this.username = username;
        this.roles = roles;
        this.sessionId = sessionId;
    }


    public LoginInfoResponse(String username, String sessionId) {
        this.username = username;
        this.sessionId = sessionId;
    }


}
