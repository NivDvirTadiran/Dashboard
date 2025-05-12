package tadiran.webnla.accdashboard.payload;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Getter
@Setter
public class SuperGroupListItemDto {
    private long superGroupId;
    private String superGroupName;
    private List<GroupListItemDto> returnArray;
}
