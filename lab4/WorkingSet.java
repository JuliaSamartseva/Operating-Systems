import java.util.HashMap;
import java.util.LinkedList;

public class WorkingSet {
  private int size;
  private LinkedList<Page> set;
  private HashMap<Page, Integer> map;

  public WorkingSet(int size) {
    this.size = size;
    set = new LinkedList<>();
    map = new HashMap<>();
  }

  // returns page index to be replaced
  // null if no need for replacement
  public Page addPage(Page page) {
    set.add(page);
    map.put(page, map.getOrDefault(page, 0) + 1);
    if (set.size() > size) {
      Page pageToRemove = set.removeFirst();
      map.put(page, map.getOrDefault(page, 0) - 1);
      return pageToRemove;
    }
    return null;
  }

  public boolean contains(Page page) {
    if (map.containsKey(page) && map.get(page) > 0) return true;
    return false;
  }
}
