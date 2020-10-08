package org.example;

import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Toolkit;
import java.io.IOException;
import java.io.PipedInputStream;
import java.util.Optional;
import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.WindowConstants;
import spos.lab1.demo.IntOps;

public class Manager {
  Function runnableF;
  Function runnableG;
  private final Thread threadF;
  private final Thread threadG;
  PipedInputStream inputStreamF = new PipedInputStream();
  PipedInputStream inputStreamG = new PipedInputStream();
  int resultF;
  int resultG;

  public Manager(int x, int y) throws IOException {
    this(x, y, IntOps::funcF, IntOps::funcG);
  }

  public Manager(int x, int y, Computation functionF, Computation functionG) throws IOException {
    runnableF = new Function(x, functionF, inputStreamF);
    runnableG = new Function(y, functionG, inputStreamG);

    threadF = new Thread(runnableF);
    threadG = new Thread(runnableG);

    setShutdownByEscapeKey();
  }

  void setShutdownByEscapeKey() {
    Runtime.getRuntime().addShutdownHook(new Thread() {
      public void run() {
        System.out.println("Cancelling the computation by the escape key.");
      }
    });
  }

  public void startComputations() throws InterruptedException {
    runnableF.registerListener(
        thread -> {
          try {
            resultF = inputStreamF.read();
            if (resultF == 0) {
              runnableG.stopThread();
            }
            runnableF.stopThread();
          } catch (IOException e) {
            e.printStackTrace();
          }
        });

    runnableG.registerListener(
        thread -> {
          try {
            resultG = inputStreamG.read();
            if (resultG == 0) {
              runnableF.stopThread();
            }
            runnableG.stopThread();
          } catch (IOException e) {
            e.printStackTrace();
          }
        });

    threadF.start();
    threadG.start();

    threadF.join();
    threadG.join();

    int finalResult = Math.min(resultF, resultG);
    System.out.println("The computed result = " + finalResult);
    runnableF.stopThread();
    runnableG.stopThread();
  }

  private JTextArea createTextArea(int width, int height, int xPos, int yPos, String text) {
    JTextArea textArea = new JTextArea();
    textArea.setBounds(xPos, yPos, width, height);
    textArea.setText(text);
    return textArea;
  }

  private JFrame initFrame(String text) {
    JFrame frame = new JFrame("Computation result");
    frame.setSize(400, 400);
    frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    frame.setLayout(new FlowLayout());
    frame.add(createTextArea(200, 200, 10, 10, text));
    Dimension dimension = Toolkit.getDefaultToolkit().getScreenSize();
    int x = (int) ((dimension.getWidth() - frame.getWidth()) / 2);
    int y = (int) ((dimension.getHeight() - frame.getHeight()) / 2);
    frame.setLocation(x, y);
    return frame;
  }
}
