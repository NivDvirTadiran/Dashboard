package tadiran.webnla.accdashboard.payload;

import java.util.List;

public class GenListResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<GenListItemDto> returnArray;

    public ResponseInfoHeaderDto getResponseInfoHeader() {
        return responseInfoHeader;
    }

    public void setResponseInfoHeader(ResponseInfoHeaderDto responseInfoHeader) {
        this.responseInfoHeader = responseInfoHeader;
    }

    public List<GenListItemDto> getReturnArray() {
        return returnArray;
    }

    public void setReturnArray(List<GenListItemDto> returnArray) {
        this.returnArray = returnArray;
    }
}
