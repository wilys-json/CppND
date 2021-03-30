#include "system.h"

#include <unistd.h>
#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>
#include <set>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::size_t;
using std::string;
using std::vector;
using std::set;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids{LinuxParser::Pids()};
  processes_.clear();
  for (auto& pid : pids) processes_.push_back(pid);
  std::sort(processes_.rbegin(), processes_.rend(), std::less<Process>());
  return processes_;
}
