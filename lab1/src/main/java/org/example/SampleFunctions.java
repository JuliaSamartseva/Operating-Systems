package org.example;

import static java.util.concurrent.TimeUnit.SECONDS;
import java.util.concurrent.TimeUnit;

public class SampleFunctions {

  public static int sampleOne(int x) throws InterruptedException, UndefinedResultException {
    SECONDS.sleep(20);
    if (x == 50) throw new UndefinedResultException("Undefined result");
    return x * 10;
  }

  public static int sampleTwo(int x) throws InterruptedException, UndefinedResultException {
    SECONDS.sleep(2);
    if (x == 20) throw new UndefinedResultException("Undefined result");
    return 15;
  }
}
