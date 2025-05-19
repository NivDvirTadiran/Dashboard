package tadiran.webnla.accdashboard.payload.response;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class SaveReportResponseDto {
    private long reportId;
    private long serviceReqId;
    private long errorCause;
}
