package org.example;

import java.io.IOException;
import java.util.logging.LogManager;

public class App {
  public static void main(String[] args) throws IOException, InterruptedException {
    LogManager.getLogManager().reset();
    System.out.println("Usual computations with shutdown key: ");
    Manager manager = new Manager(4, 5, SampleFunctions::sampleOne, SampleFunctions::sampleTwo);
    manager.startComputations();
  }
}
