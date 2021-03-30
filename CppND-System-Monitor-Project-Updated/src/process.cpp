#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(const int& pid) : ProcessParser::pid_(pid) {}

// Return this process's ID
int Process::Pid() const override { return pid_; }


// Return this process's CPU utilization
float Process::CpuUtilization() const {
  const long jiffies = ActiveJiffies(Pid()) / HERTZ;
  const long uptime =  UpTime();
  return (jiffies * 1.0) / uptime;
}


// Return the command that generated this process
string Process::Command() const override { return command_; }

// Return this process's memory utilization
string Process::Ram() override { return Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() const override { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() const override { return UpTime(pid_); }

bool Process::operator<(Process const& other) const {
  return CpuUtilization() < other.CpuUtilization();
}
