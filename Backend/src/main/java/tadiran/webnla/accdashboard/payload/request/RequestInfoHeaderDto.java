package tadiran.webnla.accdashboard.payload.request;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class RequestInfoHeaderDto {
    private long serviceReqId;
    private String ticket;
}
