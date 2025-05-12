package tadiran.webnla.accdashboard.payload;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Getter
@Setter
public class DnisListResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<DnisListItemDto> returnArray;
}
