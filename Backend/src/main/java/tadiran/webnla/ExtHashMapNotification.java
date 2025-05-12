package tadiran.webnla;

/**
 * access notification for HashMap
 * @author evgeniv
 */
public interface ExtHashMapNotification<KeyType> {
	public void beforeGet(KeyType key);
}
