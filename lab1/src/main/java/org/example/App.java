package org.example;

import java.io.IOException;
import java.util.logging.LogManager;
import spos.lab1.demo.IntOps;

public class App {
  public static void main(String[] args) throws IOException, InterruptedException {
    LogManager.getLogManager().reset();
    Manager manager = new Manager(4, 4, IntOps::funcF, IntOps::funcG);
    manager.startComputations(Mode.SHUTDOWN_PROMPT);
  }
}
