package org.example;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

public class Function implements Runnable {

  private final int input;
  private final Computation function;
  private volatile boolean running = true;
  private final List<Listener> listeners = new ArrayList<>();
  private final PipedOutputStream outputStream = new PipedOutputStream();
  private static final Logger log = Logger.getLogger(Manager.class.getName());

  public Function(int input, Computation function, PipedInputStream inputStream)
      throws IOException {
    this.input = input;
    this.function = function;
    inputStream.connect(outputStream);
  }

  @Override
  public void run() {
    try {
      while (running) {
        log.info("Start applying function.");
        int result = function.applyFunction(input);
        log.info(
            "The function was computed. Passing the "
                + result
                + " result from the function to the output stream.");
        outputStream.write(result);
        notifyListeners();
      }
    } catch (InterruptedException | IOException e) {
      e.printStackTrace();
    }
  }

  public void stopThread() {
    log.info("Stopping the thread.");
    running = false;
  }

  // Listeners functionality

  private void notifyListeners() {
    for (Listener listener : listeners) {
      listener.workDone(this);
    }
  }

  public void registerListener(Listener listener) {
    listeners.add(listener);
  }
}
