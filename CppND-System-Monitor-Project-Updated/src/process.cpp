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
  const long system_jiffies = LinuxParser::Jiffies();
  CpuUtilization(system_jiffies);
}

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
    return cpu_;
}

// Calculate & update CPU usage
void Process::CpuUtilization(const long& system_jiffies) {
  const long active_duration = LinuxParser::ActiveJiffies(Pid()) - prev_active_jiffies_;
  const long system_duration = system_jiffies - prev_system_jiffies_;
  cpu_ = active_duration * 1.0 / system_duration;
  prev_active_jiffies_ = active_duration;
  prev_system_jiffies_ = system_jiffies;
}

// Return the command that generated this process
string Process::Command() const { return LinuxParser::Command(Pid()); }
// Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(Pid()); }

// Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(Pid()); }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(Pid()); }

bool Process::operator<(Process const& other) const {
  return CpuUtilization() < other.CpuUtilization();
}
