package org.example;

import java.io.IOException;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.ArrayList;
import java.util.List;

interface Computation {
  int applyFunction(int x) throws InterruptedException;
}

interface Listener {
  void workDone(Function thread);
}

public class Function implements Runnable {

  private final int x;
  private final Computation function;
  private volatile boolean running = true;
  private List<Listener> listeners = new ArrayList<>();
  private PipedOutputStream outputStream = new PipedOutputStream();

  public Function(int x, Computation function, PipedInputStream inputStream) throws IOException {
    this.x = x;
    this.function = function;
    inputStream.connect(outputStream);
  }

  public void stopThread() {
    running = false;
  }

  @Override
  public void run() {
    try {
      while (running) {
        int result = function.applyFunction(x);
        outputStream.write(result);
        notifyListeners();
      }
    } catch (InterruptedException | IOException e) {
      e.printStackTrace();
    }
  }

  private void notifyListeners() {
    for (Listener listener : listeners) {
      listener.workDone(this);
    }
  }

  public void registerListener(Listener listener) {
    listeners.add(listener);
  }

}
