package tadiran.webnla.accdashboard.payload;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Getter
@Setter
public class GroupListResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<GroupListItemDto> returnArray;
}
