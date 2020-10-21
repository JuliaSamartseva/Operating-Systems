package org.example;

import java.io.IOException;
import java.io.PipedInputStream;
import spos.lab1.demo.IntOps;
import java.util.logging.Logger;

public class Manager {
  Function runnableF;
  Function runnableG;
  private final Thread threadF;
  private final Thread threadG;
  PipedInputStream inputStreamF = new PipedInputStream();
  PipedInputStream inputStreamG = new PipedInputStream();
  int resultF;
  int resultG;
  private static final Logger log = Logger.getLogger(Manager.class.getName());

  public Manager(int x, int y) throws IOException {
    this(x, y, IntOps::funcF, IntOps::funcG);
  }

  public Manager(int x, int y, Computation functionF, Computation functionG) throws IOException {
    runnableF = new Function(x, functionF, inputStreamF);
    runnableG = new Function(y, functionG, inputStreamG);

    threadF = new Thread(runnableF);
    threadG = new Thread(runnableG);
  }

  public void startComputations(Mode mode) throws InterruptedException {
    runnableF.registerListener(
        thread -> {
          try {
            if (inputStreamF.available() > 0) {
              resultF = inputStreamF.read();
              if (resultF == 0) {
                System.out.println(
                    "The function F has returned zero value. Stopping the application.");
                System.exit(0);
              }
              runnableF.stopThread();
            }

          } catch (IOException e) {
            e.printStackTrace();
          }
        });

    runnableG.registerListener(
        thread -> {
          try {
            if (inputStreamG.available() > 0) {
              resultG = inputStreamG.read();
              if (resultG == 0) {
                System.out.println(
                    "The function G has returned zero value. Stopping the application");
                System.exit(0);
              }
              runnableG.stopThread();
            }
          } catch (IOException e) {
            e.printStackTrace();
          }
        });

    threadF.start();
    threadG.start();

    switch (mode) {
      case SHUTDOWN_PROMPT:
        ShutdownManager.shutdownPromptListener();
        break;
      case SPECIAL_KEY_CANCELLATION:
        ShutdownManager.cancellationKeyListener();
        break;
    }

    threadF.join();
    threadG.join();

    System.out.println("The computed result = " + binaryOperation(resultF, resultG));
    System.exit(0);
  }

  private int binaryOperation(int x, int y) {
    return Math.min(x, y);
  }
}
