package org.example;

import static java.util.concurrent.TimeUnit.SECONDS;

public class SampleFunctions {

  public static int sampleF(int x) throws InterruptedException {
    if (x == 1) SECONDS.sleep(5);
    else if (x == 2) {
      SECONDS.sleep(1);
      return 0;
    } else if (x == 3) SECONDS.sleep(15);
    else if (x == 4) SECONDS.sleep(8);
    return x * 10;
  }

  public static int sampleG(int x) throws InterruptedException {
    if (x == 1) SECONDS.sleep(3);
    else if (x == 2) {
      SECONDS.sleep(1);
      return 0;
    } else if (x == 3) SECONDS.sleep(25);
    else if (x == 4) SECONDS.sleep(7);
    return 15 + x;
  }
}
