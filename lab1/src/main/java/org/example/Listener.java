package org.example;

@FunctionalInterface
interface Listener {
  void workDone(Function thread);
}
