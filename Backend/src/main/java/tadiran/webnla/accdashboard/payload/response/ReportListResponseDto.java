package tadiran.webnla.accdashboard.payload.response;

import lombok.Getter;
import lombok.Setter;
import tadiran.webnla.accdashboard.payload.request.ReportListItemDto;

import java.util.List;

@Getter
@Setter
public class ReportListResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<ReportListItemDto> returnArray;
}
