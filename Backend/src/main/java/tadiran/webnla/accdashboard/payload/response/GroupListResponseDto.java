package tadiran.webnla.accdashboard.payload.response;

import lombok.Getter;
import lombok.Setter;
import tadiran.webnla.accdashboard.payload.request.GroupListItemDto;

import java.util.List;

@Getter
@Setter
public class GroupListResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<GroupListItemDto> returnArray;
}
