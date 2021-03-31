#include "system.h"

#include <algorithm>

#include "detect_os.h"
#include "process.h"
#include "processor.h"

using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids{Pids()};
  processes_.clear();
  for (auto& pid : pids) processes_.push_back(pid);
  std::sort(processes_.rbegin(), processes_.rend(), std::less<Process>());
  return processes_;
}
