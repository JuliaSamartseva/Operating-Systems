package org.example;

import java.io.IOException;
import java.util.logging.LogManager;
import spos.lab1.demo.IntOps;

public class App {
  public static void main(String[] args) throws IOException, InterruptedException {
    LogManager.getLogManager().reset();
    Manager manager = new Manager(1, 1, SampleFunctions::sampleF, SampleFunctions::sampleG);
    manager.startComputations(Mode.SHUTDOWN_PROMPT);
  }
}
