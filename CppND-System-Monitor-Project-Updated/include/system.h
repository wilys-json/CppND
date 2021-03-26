// #ifndef SYSTEM_H
// #define SYSTEM_H
//
// #include <string>
// #include <vector>
// #include <set>
//
// #include "process.h"
// #include "processor.h"
//
// class System {
//  public:
//   Processor& Cpu();
//   std::vector<Process>& Processes();
//   void updateCpuUtilizations();
//   void updateProcesses(const std::vector<int>& pids);
//   float MemoryUtilization();
//   long UpTime();
//   int TotalProcesses();
//   int RunningProcesses();
//   std::string Kernel();
//   std::string OperatingSystem();
//
//  private:
//   Processor cpu_;
//   std::vector<Process> processes_{};
//   std::set<int> pids_{};
// };
//
// #endif

#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   // DONE: See src/system.cpp
  std::vector<Process>& Processes();  // DONE: See src/system.cpp
  float MemoryUtilization();          // DONE: See src/system.cpp
  long UpTime();                      // DONE: See src/system.cpp
  int TotalProcesses();               // DONE: See src/system.cpp
  int RunningProcesses();             // DONE: See src/system.cpp
  std::string Kernel();               // DONE: See src/system.cpp
  std::string OperatingSystem();      // DONE: See src/system.cpp

  // DONE: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif
