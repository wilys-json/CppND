#include "linux_parser.h"
#include "generic_parser.h"
#include "generic_parser.cpp"
#include <dirent.h>

#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <algorithm>

using std::accumulate;
using std::stof;
using std::stoi;
using std::stol;
using std::string;
using std::to_string;
using std::vector;


// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

long LinuxParser::SystemUpTime() {
  return GenericParser<long>::getValue(kProcDirectory + kUptimeFilename,
                                       kUptimeItemNumber);
}

string LinuxParser::SystemParser::OperatingSystem() {
  vector<Replace> inlineReplace{Replace(' ', '_'), Replace('=', ' '),
                                Replace('"', ' ')};
  vector<Replace> valueReplace{Replace('_', ' ')};
  return GenericParser<string>::getValue(kOSPath, kOSName, inlineReplace,
                                         valueReplace);
}

string LinuxParser::SystemParser::Kernel() {
  return GenericParser<string>::getValue(kProcDirectory + kVersionFilename,
                                         kKernalItemNumber);
}


// Calculate Memory utilization
float LinuxParser::SystemParser::calculateMemoryUtilization(const vector<float>& meminfo) {
  float utilization = (meminfo[kMemTotal_] - (meminfo[kMemFree_]
    + meminfo[kBuffers_] + meminfo[kCached_])) / meminfo[kMemTotal_];
  return utilization;
}

// Read and return the system memory utilization
float LinuxParser::SystemParser::MemoryUtilization() {
  return calculateMemoryUtilization(GenericParser<float>::getValues(
      kProcDirectory + kMeminfoFilename, kMemInfoRangeToRead));
}

// Read and return the system Uptime
long LinuxParser::SystemParser::UpTime() {
  return LinuxParser::SystemUpTime();
}


// Read and return the total number of processes
int LinuxParser::SystemParser::TotalProcesses() {
  return GenericParser<int>::getValue(kProcDirectory + kStatFilename,
    kTotalProcess);
}

// Read and return the number of running processes
int LinuxParser::SystemParser::RunningProcesses() {
  return GenericParser<int>::getValue(kProcDirectory + kStatFilename,
                                      kRunningProcess);
}



// Read and return CPU utilization
vector<long> LinuxParser::ProcessorParser::Utilizations() {
  return GenericParser<long>::getValues(kProcDirectory + kStatFilename, kCPUKey);
}

// Read and return the number of jiffies for the system
long LinuxParser::ProcessorParser::Jiffies() {
  vector<long> utilization = Utilizations();
  return accumulate(utilization.begin(), utilization.end(), 0) -
         (utilization[CPUStates::kGuest_] +
          utilization[CPUStates::kGuestNice_]);
}



// Read and return the number of active jiffies for the system
long LinuxParser::ProcessorParser::ActiveJiffies() {
  vector<long> utilization = Utilizations();
  return accumulate(utilization.begin(), utilization.end(), 0) -
         (utilization[CPUStates::kIdle_] + utilization[CPUStates::kIOwait_]);
}

// Read and return the number of idle jiffies for the system
long LinuxParser::ProcessorParser::IdleJiffies() {
  vector<long> utilization = Utilizations();
  return utilization[CPUStates::kIdle_] + utilization[CPUStates::kIOwait_];
}


float LinuxParser::ProcessorParser::Utilization() {
  float utilization;
  long activeJiffies = ActiveJiffies();
  long idleJiffies = IdleJiffies();

  utilization = (activeJiffies - prev_active_jiffies_) /
                (((activeJiffies - prev_active_jiffies_) +
                  (idleJiffies - prev_idle_jiffies_)) *
                 1.0);

  prev_active_jiffies_ = activeJiffies;
  prev_idle_jiffies_ = idleJiffies;

  return utilization;
}

LinuxParser::ProcessParser::ProcessParser(const int& pid) : pid_(pid) {}

int LinuxParser::ProcessParser::Pid() const { return pid_; }

// Read and return the command associated with a process
string LinuxParser::ProcessParser::Command() const{
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid_) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, value);
    return value;
  }
  return "";
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ProcessParser::ActiveJiffies() const {
  long utilization{0};
  vector<string> stats = GenericParser<string>::getValues(
      kProcDirectory + to_string(pid_) + kStatFilename);
  for (auto& info : kCpuUtilInfo) utilization += stol(stats[info]);
  return utilization;
}

// Read and return the memory used by a process
string LinuxParser::ProcessParser::Ram() {
  return to_string(GenericParser<long>::getValue(kProcDirectory + to_string(pid_) + kStatusFilename, kProcessRAM) / 1000);
}

// Read and return the user ID associated with a process
string LinuxParser::ProcessParser::Uid() const {
  return GenericParser<string>::getValue(
      kProcDirectory + to_string(pid_) + kStatusFilename, kUID);
}

// Read and return the user associated with a process
string LinuxParser::ProcessParser::User() const {
  vector<Replace> replace{Replace('x', ' '), Replace(':', ' ')};
  return GenericParser<string>::getValue(kPasswordPath, Uid(), replace, {});
}

// Read and return the uptime of a process
long LinuxParser::ProcessParser::UpTime() const {
  return LinuxParser::SystemUpTime() - stol(GenericParser<string>::getValue(
             kProcDirectory + to_string(pid_) + kStatFilename,
             kStartTimePosition)) /
         HERTZ;
}

float LinuxParser::ProcessParser::CpuUtilization() const {
  const long jiffies = ActiveJiffies() / HERTZ;
  const long uptime =  UpTime();
  return (jiffies * 1.0) / uptime;
}
