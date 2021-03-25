#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> newProcesses;
  if (!pids_.empty() && !processes_.empty()) {
      vector<int> currentPIDs = LinuxParser::Pids();
      set_difference(currentPIDs.begin(), currentPIDs.end(), pids_.begin(), pids_.end(), newProcesses.begin());
  } else {
      newProcesses = LinuxParser::Pids();
  }
  for (auto& pid : newProcesses) {
      processes_.emplace_back(Process(pid));
      pids_.insert(pid);
  }
  for (auto& process : processes_) process.CpuUtilization(LinuxParser::ActiveJiffies(process.Pid()), LinuxParser::Jiffies());
  std::sort(processes_.rbegin(), processes_.rend(), std::less<Process>());
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
