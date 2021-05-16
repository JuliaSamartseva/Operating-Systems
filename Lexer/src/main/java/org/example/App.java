package org.example;

import java.io.FileNotFoundException;
import java.io.FileReader;

public class App {
  public static void main(String[] args) throws FileNotFoundException {
    FileReader fileReader = new FileReader("test.txt");
    Lexer cLexer = new Lexer(fileReader);
    int idx = 0;
    System.out.println("List of tokens:");
    System.out.println("Output format - (start index, token class, representation)");
    while (true) {
      try {
        Type type = cLexer.yylex();
        if (type == Type.EOF) {
          break;
        }
        int len = cLexer.yytext().length();
        String target = String.format("(%d, %s, \"%s\")", idx, type.name(), cLexer.yytext());
        System.out.println(target);
        idx += len;
      } catch (Exception e) {
        e.printStackTrace();
      }
    }
  }
}
