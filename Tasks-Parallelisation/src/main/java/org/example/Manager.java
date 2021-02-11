package org.example;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.logging.Logger;
import spos.lab1.demo.IntOps;

public class Manager {
  private static final Logger log = Logger.getLogger(Manager.class.getName());
  private final Function runnableF;
  private final Function runnableG;
  private final Thread threadF;
  private final Thread threadG;
  private final PipedInputStream inputStreamF = new PipedInputStream();
  private final PipedInputStream inputStreamG = new PipedInputStream();
  private final Result result = new Result();
  private ShutdownManager shutdownManager;

  public Manager(int x, int y) throws IOException {
    this(x, y, IntOps::funcF, IntOps::funcG);
  }

  public Manager(int x, int y, Computation functionF, Computation functionG) throws IOException {
    PipedOutputStream outputStreamF = new PipedOutputStream(inputStreamF);
    PipedOutputStream outputStreamG = new PipedOutputStream(inputStreamG);

    runnableF = new Function(x, functionF, outputStreamF);
    runnableG = new Function(y, functionG, outputStreamG);

    threadF = new Thread(runnableF);
    threadG = new Thread(runnableG);
  }

  public void startComputations(Mode mode) throws InterruptedException {
    shutdownManager = new ShutdownManager(mode, result);

    Thread resultFReader =
        new Thread(
            () -> {
              try {
                log.info("Waiting for the F input stream result.");
                while (!(inputStreamF.available() > 0)) {}
                result.resultF = inputStreamF.read();
                inputStreamF.close();
                log.info("Input stream F has received the result = " + result.resultF);
                if (result.resultF.equals(0)) {
                  shutdownManager.hidePrompt();
                  System.out.println(
                      "The function F has returned zero value. Stopping the application.");
                  System.exit(0);
                }
                runnableF.stopThread();
              } catch (IOException e) {
                e.printStackTrace();
              }
            });

    Thread resultGReader =
        new Thread(
            () -> {
              try {
                log.info("Waiting for the G input stream result.");
                while (!(inputStreamG.available() > 0)) {}
                result.resultG = inputStreamG.read();
                inputStreamG.close();
                log.info("Input stream G has received the result = " + result.resultG);
                if (result.resultG.equals(0)) {
                  shutdownManager.hidePrompt();
                  System.out.println(
                      "The function G has returned zero value. Stopping the application");
                  System.exit(0);
                }
                runnableG.stopThread();
              } catch (IOException e) {
                e.printStackTrace();
              }
            });

    resultFReader.start();
    resultGReader.start();

    threadF.start();
    threadG.start();

    threadF.join();
    threadG.join();

    shutdownManager.hidePrompt();

    Thread.sleep(1000);
    System.out.println(result);
    System.out.println("The computed result = " + binaryOperation(result.resultF, result.resultG));
    System.exit(0);
  }

  private int binaryOperation(int x, int y) {
    return Math.min(x, y);
  }
}
