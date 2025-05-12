package tadiran.webnla.model;


import lombok.Getter;
import lombok.Setter;

/**
 * <h2>BaseResponse</h2>
 *
 * @author aek
 */
@Getter
@Setter
public class Prop {
    private String propName;

    private String propValue;

    public Prop(String propName, String propValue) {
        this.propName = propName;
        this.propValue = propValue;
    }
}
