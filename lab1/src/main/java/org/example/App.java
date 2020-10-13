package org.example;

import java.io.IOException;
import java.util.logging.LogManager;
import spos.lab1.demo.IntOps;

public class App {
  public static void main(String[] args) throws IOException, InterruptedException {
    LogManager.getLogManager().reset();
    System.out.println("Usual computations with shutdown key: ");
    Manager manager = new Manager(10, 10, IntOps::funcF, SampleFunctions::sampleTwo);
    manager.startComputations();
  }
}
