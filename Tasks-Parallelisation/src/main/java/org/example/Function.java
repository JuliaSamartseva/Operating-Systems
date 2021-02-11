package org.example;

import java.io.IOException;
import java.io.PipedOutputStream;
import java.util.logging.Logger;

public class Function implements Runnable {

  private static final Logger log = Logger.getLogger(Manager.class.getName());
  private final int input;
  private final Computation function;
  private final PipedOutputStream outputStream;
  private volatile boolean running = true;

  public Function(int input, Computation function, PipedOutputStream outputStream) {
    this.input = input;
    this.function = function;
    this.outputStream = outputStream;
  }

  @Override
  public void run() {
    try {
      while (running) {
        log.info("Start applying function. " + Thread.currentThread().getName());
        int result = function.applyFunction(input);
        log.info(
            "The function was computed. Passing the "
                + result
                + " result from the function to the output stream.");
        outputStream.write(result);
        outputStream.close();
        running = false;
      }
    } catch (InterruptedException | IOException e) {
      e.printStackTrace();
    }
  }

  public void stopThread() {
    log.info("Stopping the thread.");
    running = false;
  }
}
