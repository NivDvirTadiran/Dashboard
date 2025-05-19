package tadiran.webnla.accdashboard.payload.request;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class DataItemDto {
    private String ersid;
    private String value;
    private String ersname;
    private Long bgcolor; // Use Long for optional unsignedInt
}
