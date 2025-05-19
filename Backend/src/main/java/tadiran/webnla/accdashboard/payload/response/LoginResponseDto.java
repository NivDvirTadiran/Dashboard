package tadiran.webnla.accdashboard.payload.response;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class LoginResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private String ticket;
    private long loginEnable;
    private long supLevel;
}
