#include "ncurses_display.h"
#include "system.h"
#include "utest.h"
int main() {

  System system;
  NCursesDisplay::Display(system);
  // Test::utest_linux_parser();
}
