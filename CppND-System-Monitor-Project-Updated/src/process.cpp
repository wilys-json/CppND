#include "process.h"

Process::Process(const int& pid) : ProcessParser(pid) {}

bool Process::operator<(Process const& other) const {
  return CpuUtilization() < other.CpuUtilization();
}
