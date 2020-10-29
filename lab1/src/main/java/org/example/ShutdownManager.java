package org.example;

import java.awt.FlowLayout;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.Timer;

public class ShutdownManager {
  private static int seconds = 15;
  private final Mode mode;
  private final Result result;
  private JFrame frame;
  private Timer timer;

  public ShutdownManager(Mode mode, Result result) {
    this.mode = mode;
    this.result = result;
    switch (mode) {
      case SHUTDOWN_PROMPT:
        frame = new JFrame("Shutdown prompt");
        shutdownPromptListener();
        break;
      case SPECIAL_KEY_CANCELLATION:
        cancellationKeyListener();
        break;
    }
  }

  public void hidePrompt() {
    if (mode == Mode.SHUTDOWN_PROMPT) frame.setVisible(false);
  }

  private void cancellationKeyListener() {
    Runnable runnable =
        () -> {
          System.out.print("---------------\n" + "Press 1 to stop \n");
          Scanner scanner = new Scanner(System.in);
          int response = scanner.nextInt();
          if (response == 1) {
            System.out.println("Cancellation key was clicked, stopping the application.");
            System.exit(0);
          }
        };

    Thread thread = new Thread(runnable);
    thread.start();
  }

  private void shutdownPromptListener() {
    Runnable runnable =
        () -> {
          System.out.print("---------------\n" + "Press 1 to show the shutdown prompt \n");
          Scanner scanner = new Scanner(System.in);
          int response = scanner.nextInt();
          if (response == 1) showShutdownPrompt();
        };

    Thread thread = new Thread(runnable);
    thread.start();
  }

  private void showShutdownPrompt() {
    frame.setAlwaysOnTop(true);
    JPanel panel = new JPanel();
    panel.setLayout(new FlowLayout());
    JLabel label = new JLabel("Shutdown prompt");

    JButton continueButton = createContinueButton();
    JButton cancelButton = createCancelButton();
    cancelButton.addActionListener(
        e -> {
          hidePrompt();
          System.out.println(
              "The computation is terminated, because the cancel button was clicked.");
          printFunctionResultsState();
          System.exit(0);
        });
    JLabel timerLabel = createTimerLabel();

    panel.add(label);
    panel.add(timerLabel);
    panel.add(continueButton);
    panel.add(cancelButton);
    frame.add(panel);

    frame.setSize(200, 200);
    frame.setLocationRelativeTo(null);
    frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    frame.setVisible(true);
  }

  private void printFunctionResultsState() {
    System.out.println(result);
  }

  private JLabel createTimerLabel() {
    JLabel timerLabel = new JLabel();
    timer =
        new Timer(
            1000,
            e -> {
              seconds--;
              long second =
                  TimeUnit.SECONDS.toSeconds(seconds) - (TimeUnit.SECONDS.toMinutes(seconds) * 60);
              timerLabel.setText(second + " second(s) are left");
              if (seconds == 0) {
                frame.setVisible(false);
                System.out.println(
                    "No response was given on 15 seconds in the shutdown prompt, so the computation is terminated.");
                System.exit(0);
              }
            });
    timer.start();
    return timerLabel;
  }

  private JButton createContinueButton() {
    JButton continueButton = new JButton("Continue");
    continueButton.addActionListener(
        e -> {
          timer.stop();
          frame.setVisible(false);
          System.out.println("The continue button was clicked, the computation continues.");
        });
    return continueButton;
  }

  private JButton createCancelButton() {
    return new JButton("Cancel");
  }
}
