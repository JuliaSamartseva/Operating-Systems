package org.example;

import java.io.IOException;

public class App {
  public static void main(String[] args) throws IOException, InterruptedException {
    System.out.println("Usual computations with shutdown key: ");
    Manager manager = new Manager(4, 5);
    manager.startComputations();
    
    Manager sampleFunctionsManager = new Manager(4, 6, (int) -> )
  }
}
