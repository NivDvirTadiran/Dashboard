package tadiran.webnla.accdashboard.payload.request;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class DnisListItemDto {
    private long dnisId;
    private String dnisName;
    private String dnisNumber;
    private long dnisType;
}
