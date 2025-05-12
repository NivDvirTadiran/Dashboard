package tadiran.webnla.accdashboard.payload;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

@Getter
@Setter
public class DataItemRowDto {
    private List<DataItemDto> returnArray;
}
