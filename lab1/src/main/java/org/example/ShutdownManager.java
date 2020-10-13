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
  static JFrame frame;
  static Timer timer;

  public static void shutdownListener() {
    Runnable runnable =
        () -> {
          System.out.print("---------------\n" + "Press 1 to stop \n");
          Scanner scanner = new Scanner(System.in);
          int response = scanner.nextInt();
          if (response == 1) System.exit(0);
        };

    Thread thread = new Thread(runnable);
    thread.start();
  }

  public static void escapeKeyListener() {
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

  static int seconds = 15;

  private static void showShutdownPrompt() {
    frame = new JFrame("Shutdown prompt");
    frame.setAlwaysOnTop(true);
    JPanel panel = new JPanel();
    panel.setLayout(new FlowLayout());
    JLabel label = new JLabel("Shutdown prompt");

    JButton continueButton = createContinueButton();
    JButton cancelButton = createCancelButton();
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

  static JLabel createTimerLabel() {
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

  static JButton createContinueButton() {
    JButton continueButton = new JButton("Continue");
    continueButton.addActionListener(
        e -> {
          timer.stop();
          frame.setVisible(false);
          System.out.println("The continue button was clicked, the computation continues.");
        });
    return continueButton;
  }

  static JButton createCancelButton() {
    JButton cancelButton = new JButton("Cancel");
    cancelButton.addActionListener(
        e -> {
          frame.setVisible(false);
          System.out.println(
              "The computation is terminated, because the cancel button was clicked.");
          System.exit(0);
        });
    return cancelButton;
  }
}
