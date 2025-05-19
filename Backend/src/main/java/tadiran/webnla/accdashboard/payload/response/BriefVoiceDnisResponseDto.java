package tadiran.webnla.accdashboard.payload.response;

import tadiran.webnla.accdashboard.payload.request.DataItemRowDto;

import java.util.List;

public class BriefVoiceDnisResponseDto {
    private ResponseInfoHeaderDto responseInfoHeader;
    private List<DataItemRowDto> returnMatrix;

    public ResponseInfoHeaderDto getResponseInfoHeader() {
        return responseInfoHeader;
    }

    public void setResponseInfoHeader(ResponseInfoHeaderDto responseInfoHeader) {
        this.responseInfoHeader = responseInfoHeader;
    }

    public List<DataItemRowDto> getReturnMatrix() {
        return returnMatrix;
    }

    public void setReturnMatrix(List<DataItemRowDto> returnMatrix) {
        this.returnMatrix = returnMatrix;
    }
}
