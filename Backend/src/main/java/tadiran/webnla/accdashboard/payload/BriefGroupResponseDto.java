package tadiran.webnla.accdashboard.payload;

import java.util.List;

public class BriefGroupResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<BlockItemDto> returnArrayOfBlocks; // Changed from List<DataItemRowDto> returnMatrix

    // Getters and Setters
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
