package tadiran.webnla.accdashboard.payload.response;

import lombok.Getter;
import lombok.Setter;
import tadiran.webnla.accdashboard.payload.request.DnisListItemDto;

import java.util.List;

@Getter
@Setter
public class DnisListResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<DnisListItemDto> returnArray;
}
