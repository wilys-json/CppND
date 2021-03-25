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

Process::Process(const int& pid) : pid_(pid) {}

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpuUtil_; }

// Calculate & update CPU usage
void Process::CpuUtilization(const long& active_jiff, const long& system_jiff) {
    cpuUtil_ = static_cast<float>((active_jiff - prev_active_jiffies_)) / (system_jiff - prev_system_jiffies_);
    prev_active_jiffies_ = active_jiff;
    prev_system_jiffies_ = system_jiff;
}

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }
// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(Pid()); }

bool Process::operator<(Process const& other) const {
  return CpuUtilization() < other.CpuUtilization();
}
