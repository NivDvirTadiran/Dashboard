package tadiran.webnla.accdashboard.payload.request;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class ReportListItemDto {
    private long reportId;
    private long isPublic;
    private String reportName;
    private String renderName;
    private String params;
}
