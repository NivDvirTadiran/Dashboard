package tadiran.webnla.accdashboard.payload.response;

import lombok.Getter;
import lombok.Setter;
import tadiran.webnla.accdashboard.payload.request.SuperGroupListItemDto;

import java.util.List;

@Getter
@Setter
public class SuperGroupListResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<SuperGroupListItemDto> returnArray;
}
