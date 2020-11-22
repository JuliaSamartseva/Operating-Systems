// Run() is called from Scheduling.main() and is where
// the scheduling algorithm written by the user resides.
// User modification should occur within the Run() function.

import java.util.LinkedList;
import java.util.Queue;
import java.util.Vector;
import java.io.*;

public class SchedulingAlgorithm {

  public static Results run(int runtime, Vector processVector, Results result, int quant, String resultsFile) {
    int i = 0;
    int comptime = 0;
    int size = processVector.size();
    int completed = 0;

    Queue<sProcess> processQueue = new LinkedList<sProcess>();
    for (int j = 0; j < processVector.size(); j++) {
      sProcess process = (sProcess) processVector.elementAt(j);
      process.id = j;
      processQueue.add(process);
    }

    result.schedulingType = "Interactive";
    result.schedulingName = "Cyclic";
    try {
      PrintStream out = new PrintStream(new FileOutputStream(resultsFile));

      repeat:
      while (comptime < runtime) {
        sProcess process = processQueue.remove();
        outputProcessData(out, process, "registered...");
        int currentProcessRunningTime = 0;

        while (currentProcessRunningTime < quant) {

          if (process.cpudone == process.cputime) {
            completed++;
            process.isCompleted = true;
            outputProcessData(out, process, "completed...");
            comptime += currentProcessRunningTime;
            if (completed == size) {
              result.compuTime = comptime;
              out.close();
              return result;
            }
            continue repeat;
          }

          if (process.ioblocking == process.ionext) {
            outputProcessData(out, process, "I/O blocked...");
            process.numblocked++;
            process.ionext = 0;
            sProcess previousProcess = process;
            processQueue.add(previousProcess);
            continue repeat;
          }

          process.cpudone++;
          if (process.ioblocking > 0) {
            process.ionext++;
          }
          currentProcessRunningTime++;

        }
        if (!process.isCompleted) processQueue.add(process);
      }

      out.close();
    } catch (IOException e) {
      e.printStackTrace();
    }
    result.compuTime = comptime;
    return result;
  }

  private static void outputProcessData(PrintStream out, sProcess process, String status) {
    out.println(
            "Process: "
                    + process.id
                    + " "
                    + status
                    + " ("
                    + process.cputime
                    + " "
                    + process.ioblocking
                    + " "
                    + process.cpudone
                    + " "
                    + process.cpudone
                    + ")");
  }
}
