#include "ncurses_display.h"
#include "system.h"
#include <iostream>
int main() {
  System system;
  NCursesDisplay::Display(system);
  // for (auto& process : system.Processes()) std::cout << process.CpuUtilization() << std::endl;

}
