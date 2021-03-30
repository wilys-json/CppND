#include "linux_parser.h"
#include "generic_parser.h"

#include <dirent.h>

#include <iostream>
#include <numeric>
#include <string>
#include <vector>

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

string LinuxParser::SystemParser::OperatingSystem() override {
  vector<Replace> inlineReplace{Replace(' ', '_'), Replace('=', ' '),
                                Replace('"', ' ')};
  vector<Replace> valueReplace{Replace('_', ' ')};
  return GenericParser<string>::getValue(kOSPath, kOSName, inlineReplace,
                                         valueReplace);
}

string LinuxParser::SystemParser::Kernel() override {
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
float LinuxParser::SystemParser::MemoryUtilization() override {
  return calculateMemoryUtilization(GenericParser<float>::getValues(
      kProcDirectory + kMeminfoFilename, kMemInfoRangeToRead));
}

// Read and return the system Uptime
long LinuxParser::SystemParser::UpTime() override {
  return GenericParser<long>::getValue(kProcDirectory + kUptimeFilename,
                                       kUptimeItemNumber);
}


// Read and return the total number of processes
int LinuxParser::SystemParser::TotalProcesses() override {
  return GenericParser<int>::getValue(kProcDirectory + kStatFilename,
    kTotalProcess);
}

// Read and return the number of running processes
int LinuxParser::SystemParser::RunningProcesses() override {
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


// Read and return the command associated with a process
string LinuxParser::ProcessParser::Command() override {
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid_) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, value);
    return value;
  }
  return "";
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ProcessParser::ActiveJiffies(int pid) {
  long utilization{0};
  vector<string> stats = GenericParser<string>::getValues(
      kProcDirectory + to_string(pid) + kStatFilename);
  for (auto& info : kCpuUtilInfo) utilization += stol(stats[info]);
  return utilization;
}

// Read and return the memory used by a process
string LinuxParser::ProcessParser::Ram(int pid) {
  return to_string(GenericParser<long>::getValue(kProcDirectory + to_string(pid) + kStatusFilename, kProcessRAM) / 1000);
}

// Read and return the user ID associated with a process
string LinuxParser::ProcessParser::Uid(int pid) {
  return GenericParser<string>::getValue(
      kProcDirectory + to_string(pid) + kStatusFilename, kUID);
}

// Read and return the user associated with a process
string LinuxParser::ProcessParser::User(int pid) {
  vector<Replace> replace{Replace('x', ' '), Replace(':', ' ')};
  return GenericParser<string>::getValue(kPasswordPath, Uid(pid), replace, {});
}

// Read and return the uptime of a process
long LinuxParser::ProcessParser::UpTime(int pid) {
  return UpTime() - stol(GenericParser<string>::getValue(
             kProcDirectory + to_string(pid) + kStatFilename,
             kStartTimePosition)) /
         HERTZ;
}
