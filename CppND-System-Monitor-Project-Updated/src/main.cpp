#include "ncurses_display.h"
#include "system.h"
// #include "linux_parser.h"
// #include <iostream>
// #include "utest.h"  // unitest

int main() {
  // Test::utest_linux_parser();
  System system;
  NCursesDisplay::Display(system);
}
