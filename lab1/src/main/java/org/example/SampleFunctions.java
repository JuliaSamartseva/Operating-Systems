package org.example;

import java.util.concurrent.TimeUnit;

public class SampleFunctions {
  public static int sampleOne(int x) throws InterruptedException {
    TimeUnit.SECONDS.sleep(20);
    return x * 10;
  }

  public static int sampleTwo(int x) throws InterruptedException {
    TimeUnit.SECONDS.sleep(2);
    return 15;
  }
}
