/* It is in this file, specifically the replacePage function that will
   be called by MemoryManagement when there is a page fault.  The
   users of this program should rewrite PageFault to implement the
   page replacement algorithm.
*/

// This PageFault file is an example of the FIFO Page Replacement
// Algorithm as described in the Memory Management section.

import java.util.*;

public class PageFault {

  /**
   * The page replacement algorithm for the memory management sumulator. This method gets called
   * whenever a page needs to be replaced.
   *
   * <p>The page replacement algorithm included with the simulator is FIFO (first-in first-out). -> Working Set A
   * while or for loop should be used to search through the current memory contents for a canidate
   * replacement page. In the case of FIFO the while loop is used to find the proper page while
   * making sure that virtPageNum is not exceeded.
   *
   * <pre>
   *   Page page = ( Page ) mem.elementAt( oldestPage )
   * </pre>
   *
   * This line brings the contents of the Page at oldestPage (a specified integer) from the mem
   * vector into the page object. Next recall the contents of the target page, replacePageNum. Set
   * the physical memory address of the page to be added equal to the page to be removed.
   *
   * <pre>
   *   controlPanel.removePhysicalPage( oldestPage )
   * </pre>
   *
   * Once a page is removed from memory it must also be reflected graphically. This line does so by
   * removing the physical page at the oldestPage value. The page which will be added into memory
   * must also be displayed through the addPhysicalPage function call. One must also remember to
   * reset the values of the page which has just been removed from memory.
   *
   * @param mem is the vector which contains the contents of the pages in memory being simulated.
   *     mem should be searched to find the proper page to remove, and modified to reflect any
   *     changes.
   * @param virtPageNum is the number of virtual pages in the simulator (set in Kernel.java).
   * @param replacePageNum is the requested page which caused the page fault.
   * @param controlPanel represents the graphical element of the simulator, and allows one to modify
   *     the current display.
   */

  public static void replacePage(
      Vector mem, int virtPageNum, int replacePageNum, ControlPanel controlPanel, WorkingSet set) {
    int firstPage = -1;
    boolean mapped = false;
    int removeIndex = -1;
    int removeIndexIfAllInSet = -1;
    Page nextpage = (Page) mem.elementAt(replacePageNum);
    Page pageToRemove = set.addPage(nextpage);

    for (int count = 0; count < virtPageNum; count++) {
      Page page = (Page) mem.elementAt(count);

      if (page.physical != -1) {
        if (!set.contains(page)) removeIndex = count;
      }

      if (page == pageToRemove) removeIndexIfAllInSet = count;
    }

    if (removeIndex == -1) {
      removeIndex = removeIndexIfAllInSet;
    }

    // Page to remove
    Page removedPage = (Page) mem.elementAt(removeIndex);
    controlPanel.removePhysicalPage(removeIndex);

    // Replacing the page in the control pannel
    nextpage.physical = removedPage.physical;
    controlPanel.addPhysicalPage(nextpage.physical, replacePageNum);
    removedPage.inMemTime = 0;
    removedPage.lastTouchTime = 0;
    removedPage.R = 0;
    removedPage.M = 0;
    removedPage.physical = -1;
  }
}
