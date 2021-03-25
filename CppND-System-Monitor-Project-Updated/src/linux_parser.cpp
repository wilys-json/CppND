#include "linux_parser.h"

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

string LinuxParser::OperatingSystem() {
  vector<Replace> inlineReplace{Replace(' ', '_'), Replace('=', ' '),
                                Replace('"', ' ')};
  vector<Replace> valueReplace{Replace('_', ' ')};
  string os = GenericParser<string>::getValue(kOSPath, kOSName, inlineReplace,
                                         valueReplace);
  return os;
}

string LinuxParser::Kernel() {
  string kernel = GenericParser<string>::getValue(kProcDirectory + kVersionFilename,
                                         kKernalItemNumber);
  return kernel;
}

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

// Caculate Memory utilization
float LinuxParser::calculateMemoryUtilization(const vector<float>& meminfo) {
  float utilization = (meminfo[kMemTotal_] - (meminfo[kMemFree_]
    + meminfo[kBuffers_] + meminfo[kCached_])) / meminfo[kMemTotal_];
  return utilization;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  vector<float> meminfo = GenericParser<float>::getValues(
      kProcDirectory + kMeminfoFilename, kMemInfoRangeToRead);
  return calculateMemoryUtilization(meminfo);
}

// Read and return the system Uptime
long LinuxParser::UpTime() {
  long uptime = GenericParser<long>::getValue(kProcDirectory + kUptimeFilename,
                                       kUptimeItemNumber);
  return uptime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<long> utilization = CpuUtilization();
  return accumulate(utilization.begin(), utilization.end(), 0) -
         (utilization[CPUStates::kGuest_] +
          utilization[CPUStates::kGuestNice_]);
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long utilization{0};
  vector<string> stats = GenericParser<string>::getValues(
      kProcDirectory + to_string(pid) + kStatFilename);
  for (auto& info : kCpuUtilInfo) utilization += stol(stats[info]);
  return utilization;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<long> utilization = CpuUtilization();
  long jiffies = accumulate(utilization.begin(), utilization.end(), 0) -
         (utilization[CPUStates::kIdle_] + utilization[CPUStates::kIOwait_]);
  return jiffies;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<long> utilization = CpuUtilization();
  long jiffies = utilization[CPUStates::kIdle_] + utilization[CPUStates::kIOwait_];
  return jiffies;
}

// Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  vector<long> utilizations = GenericParser<long>::getValues(kProcDirectory
    + kStatFilename, kCPUKey);
  return utilizations;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalproc = GenericParser<int>::getValue(kProcDirectory + kStatFilename,
    kTotalProcess);
  return totalproc;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int runningproc = GenericParser<int>::getValue(kProcDirectory + kStatFilename,
                                      kRunningProcess);
  return runningproc;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, value);
    return value;
  }
  return "";
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string ram = to_string(
      GenericParser<long>::getValue(
          kProcDirectory + to_string(pid) + kStatusFilename, kProcessRAM) /
      1024);
  return ram;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string uid = GenericParser<string>::getValue(
      kProcDirectory + to_string(pid) + kStatusFilename, kUID);
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  vector<Replace> replace{Replace('x', ' '), Replace(':', ' ')};
  string user = GenericParser<string>::getValue(kPasswordPath, Uid(pid), replace, {});
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long uptime = UpTime() - stol(GenericParser<string>::getValue(
             kProcDirectory + to_string(pid) + kStatFilename,
             kStartTimePosition)) /
         HERTZ;
  return uptime;
}

// Function templates
template <typename T>
T LinuxParser::GenericParser<T>::getValue(const string& filename,
                                                const int& position) {
  string line;
  std::ifstream filestream(filename);
  T value = T();
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < position; i++) linestream >> value;
    return value;
  }
  return value;
}

template <typename T>
T LinuxParser::GenericParser<T>::getValue(const string& filename,
                                                const string& targetKey) {
  string line, key;
  std::ifstream filestream(filename);
  T value = T();
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == targetKey) return value;
      }
    }
  }
  return value;
}

template <typename T>
T LinuxParser::GenericParser<T>::getValue(
    const std::string& filename, const std::string& targetKey,
    const std::vector<Replace>& inlineReplace,
    const std::vector<Replace>& valueReplace) {
  string line, key;
  std::ifstream filestream(filename);
  T value = T();
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (!inlineReplace.empty()) {
        for (auto& lreplace : inlineReplace) {
          std::replace(line.begin(), line.end(), lreplace.from, lreplace.to);
        }
      }
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == targetKey) {
          if (!valueReplace.empty()) {
            for (auto& vreplace : valueReplace) {
              std::replace(line.begin(), line.end(), vreplace.from,
                           vreplace.to);
            }
          }
          return value;
        }
      }
    }
  }
  return value;
}

template <typename T>
vector<T> LinuxParser::GenericParser<T>::getValues(
    const string& filename, const string& targetKey) {
  string line, key;
  std::ifstream filestream(filename);
  T value;
  vector<T> collector{};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == targetKey) {
          while (linestream >> value) collector.emplace_back(value);
          return collector;
        }
      }
    }
  }
  return collector;
}

template <typename T>
vector<T> LinuxParser::GenericParser<T>::getValues(const string& filename,
                                                         const int& range) {
  string line, key;
  std::ifstream filestream(filename);
  T value;
  vector<T> collector{};
  if (filestream.is_open()) {
    for (int i = 0; i < range; i++) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
      collector.emplace_back(value);
    }
    return collector;
  }
  return collector;
}

template <typename T>
vector<T> LinuxParser::GenericParser<T>::getValues(
    const string& filename) {
  string line;
  std::ifstream filestream(filename);
  T value;
  vector<T> collector{};
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) collector.emplace_back(value);
    return collector;
  }
  return collector;
}
