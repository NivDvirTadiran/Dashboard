package tadiran.webnla.accdashboard.payload.request;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class SaveReportRequestDto {
    private RequestInfoHeaderDto requestInfoHeader;
    private ReportListItemDto requestItem;
}
