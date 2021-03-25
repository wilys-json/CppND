#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(const int& pid) : pid_(pid) {
  user_ = LinuxParser::User(pid);
  command_ = LinuxParser::Command(pid);
  uptime_ = LinuxParser::UpTime(pid);
  ram_ = LinuxParser::Ram(pid);
}

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
  try {
    return LinuxParser::ActiveJiffies(Pid()) /
           (LinuxParser::UpTime() - UpTime());
  } catch (...) {
    return 0;
  }
}

// Return the command that generated this process
string Process::Command() const { return command_; }
// Return this process's memory utilization
string Process::Ram() const { return ram_; }

// Return the user (name) that generated this process
string Process::User() const { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return uptime_; }

bool Process::operator<(Process const& other) const {
  return CpuUtilization() < other.CpuUtilization();
}
