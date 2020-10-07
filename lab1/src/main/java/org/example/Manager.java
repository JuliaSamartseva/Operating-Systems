package org.example;

import java.io.IOException;
import java.io.PipedInputStream;
import java.util.Optional;
import spos.lab1.demo.IntOps;

public class Manager {
  Function runnableF;
  Function runnableG;
  private final Thread threadF;
  private final Thread threadG;
  PipedInputStream inputStreamF;
  PipedInputStream inputStreamG;
  Optional<Integer> resultF;
  Optional<Integer> resultG;

  public Manager(int x, int y) throws IOException {
    runnableG = new Function(x, IntOps::funcG, inputStreamG);
    runnableF = new Function(y, IntOps::funcF, inputStreamF);

    threadF = new Thread(runnableF);
    threadG = new Thread(runnableG);
  }

  public void startComputations() {
    runnableF.registerListener(new Listener() {
      @Override
      public void workDone(Function thread) {
        try {
          resultF = Optional.of(inputStreamF.read());
          if (resultF.equals(Optional.of(0))) {
            runnableG.stopThread();
          }
        } catch (IOException e) {
          e.printStackTrace();
        }
      }
    });

    runnableG.registerListener(new Listener() {
      @Override
      public void workDone(Function thread) {
        try {
          resultG = Optional.of(inputStreamG.read());
          if (resultG.equals(Optional.of(0))) {
            runnableF.stopThread();
          }
        } catch (IOException e) {
          e.printStackTrace();
        }
      }
    });

    threadF.start();
    threadG.start();

  }
}
