package tadiran.webnla.accdashboard.payload.request;

import java.util.List;

public class BlockItemDto {
    private long id; // Corresponds to xsd:unsignedInt
    private String name;
    private List<DataItemDto> returnArray;

    // Getters and Setters
    public long getId() {
        return id;
    }

    public void setId(long id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public List<DataItemDto> getReturnArray() {
        return returnArray;
    }

    public void setReturnArray(List<DataItemDto> returnArray) {
        this.returnArray = returnArray;
    }
}
