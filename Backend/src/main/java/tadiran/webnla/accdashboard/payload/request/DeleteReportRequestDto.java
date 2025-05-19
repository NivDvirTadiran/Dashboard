package tadiran.webnla.accdashboard.payload.request;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class DeleteReportRequestDto {
    private RequestInfoHeaderDto requestInfoHeader;
    private long reportId;
}
