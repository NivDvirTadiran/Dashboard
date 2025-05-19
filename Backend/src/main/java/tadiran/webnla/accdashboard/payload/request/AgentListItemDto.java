package tadiran.webnla.accdashboard.payload.request;

import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
public class AgentListItemDto {
    private long agentId;
    private String agentName;
    private String agentNumber;
}
