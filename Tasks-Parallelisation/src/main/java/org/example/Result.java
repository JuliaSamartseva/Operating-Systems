package org.example;

public class Result {
  public Integer resultF = null;
  public Integer resultG = null;

  @Override
  public String toString() {
    String result;

    if (resultF == null) result = "Function F is still computing the result.";
    else result = "Function F has been computed, F(x) = " + resultF;

    if (resultG == null) result += "\nFunction G is still computing the result.";
    else result += "\nFunction G has been computed, G(x) = " + resultG;

    return result;
  }
}
