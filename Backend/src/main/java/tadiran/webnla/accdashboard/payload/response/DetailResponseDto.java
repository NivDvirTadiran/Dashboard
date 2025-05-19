package tadiran.webnla.accdashboard.payload.response;

import tadiran.webnla.accdashboard.payload.request.BlockItemDto;

import java.util.List;

public class DetailResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<BlockItemDto> returnArrayOfBlocks;

    public ResponseInfoHeaderDto getResponseInfoHeader() {
        return responseInfoHeader;
    }

    public void setResponseInfoHeader(ResponseInfoHeaderDto responseInfoHeader) {
        this.responseInfoHeader = responseInfoHeader;
    }

    public List<BlockItemDto> getReturnArrayOfBlocks() {
        return returnArrayOfBlocks;
    }

    public void setReturnArrayOfBlocks(List<BlockItemDto> returnArrayOfBlocks) {
        this.returnArrayOfBlocks = returnArrayOfBlocks;
    }
}
