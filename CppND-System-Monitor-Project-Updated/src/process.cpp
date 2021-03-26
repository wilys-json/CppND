// #include "process.h"
//
// #include <unistd.h>
//
// #include <cctype>
// #include <sstream>
// #include <string>
// #include <vector>
//
// #include "linux_parser.h"
//
// using std::string;
// using std::to_string;
// using std::vector;
//
// Process::Process(const int& pid) : pid_(pid) {}
//
// // Return this process's ID
// int Process::Pid() const { return pid_; }
//
// // Return this process's CPU utilization
// float Process::CpuUtilization() const { return cpu_; }
//
// // Calculate & update CPU usage
// void Process::CpuUtilization(long active_jiffies, long system_jiffies) {
//   long active_duration = active_jiffies - prev_active_jiffies_;
//   long system_duration = system_jiffies - prev_system_jiffies_;
//   cpu_ = static_cast<float>(active_duration) / system_duration;
//   prev_active_jiffies_ = active_duration;
//   prev_system_jiffies_ = system_jiffies;
// }
//
// // Return the command that generated this process
// string Process::Command() { return LinuxParser::Command(pid_); }
// // Return this process's memory utilization
// string Process::Ram() { return LinuxParser::Ram(pid_); }
//
// // Return the user (name) that generated this process
// string Process::User() { return LinuxParser::User(pid_); }
//
// // Return the age of this process (in seconds)
// long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }
//
// bool Process::operator<(Process const& other) const {
//   return CpuUtilization() < other.CpuUtilization();
// }


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

// Add constructor for Process
Process::Process(int pid) : pid_(pid) {}

// DONE: Return this process's ID
int Process::Pid() const { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_; }

void Process::CpuUtilization(long active_ticks, long system_ticks) {
  long duration_active{active_ticks - cached_active_ticks_};
  long duration{system_ticks - cached_system_ticks_};
  cpu_ = static_cast<float>(duration_active) / duration;
  cached_active_ticks_ = active_ticks;
  cached_system_ticks_ = system_ticks;
}

// DONE: Return the command that generated this process
string Process::Command() const { return LinuxParser::Command(Pid()); }

// DONE: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(Pid()); }

// DONE: Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(Pid()); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(Pid()); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator>(const Process& a) const {
  return CpuUtilization() > a.CpuUtilization();
}

bool Process::operator<(const Process& a) const {
  return CpuUtilization() < a.CpuUtilization();
}
