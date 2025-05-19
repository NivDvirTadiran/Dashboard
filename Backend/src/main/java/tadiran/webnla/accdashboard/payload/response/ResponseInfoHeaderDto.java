package tadiran.webnla.accdashboard.payload.response;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class ResponseInfoHeaderDto {
    private long serviceReqId;
    private long errorCause;
    private String serversInfo;
}
