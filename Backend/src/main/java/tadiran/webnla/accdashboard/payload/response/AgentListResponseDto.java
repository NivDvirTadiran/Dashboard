package tadiran.webnla.accdashboard.payload.response;

import lombok.Getter;
import lombok.Setter;
import tadiran.webnla.accdashboard.payload.request.AgentListItemDto;

import java.util.List;

@Getter
@Setter
public class AgentListResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<AgentListItemDto> returnArray;
}
