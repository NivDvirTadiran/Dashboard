package tadiran.webnla.accdashboard.payload.request;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class GroupListItemDto {
    private long grpId;
    private String grpName;
    private String grpNumber;
    private long grpEmailEnable;
}
