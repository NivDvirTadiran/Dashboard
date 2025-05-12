package tadiran.webnla.accdashboard.payload;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Getter
@Setter
public class SuperGroupListResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<SuperGroupListItemDto> returnArray;
}
