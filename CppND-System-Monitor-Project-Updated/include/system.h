#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <set>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  void updateCpuUtilizations();
  void updateProcesses(const std::vector<int>& pids);
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

 private:
  Processor cpu_;
  std::vector<Process> processes_{};
  std::set<int> pids_{};
};

#endif
