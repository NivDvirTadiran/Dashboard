package tadiran.webnla.accdashboard.payload.response;

import lombok.Getter;
import lombok.Setter;
import tadiran.webnla.accdashboard.payload.request.DataItemRowDto;

import java.util.List;

@Getter
@Setter
public class BriefAgentsResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<DataItemRowDto> returnMatrix;
}
