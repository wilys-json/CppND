#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;
using std::runtime_error;

template <typename T>
const T LinuxParser::GenericParser<T>::getPosition(const string &filename,
    const int &position) {
    string line;
    std::ifstream filestream(filename);
    T value = T();
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        for (int i = 0; i < position; i++) {
            linestream >> value;
        }
        return value;
    }
    return value;
}


template <typename T>
const T LinuxParser::GenericParser<T>::getValue(const string &filename,
    const string& targetKey) {
    string line, key;
    std::ifstream filestream(filename);
    T value = T();
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
              if(key == targetKey) return value;
            }
        }
    }
    return value;
}


template <typename T>
const vector<T> LinuxParser::GenericParser<T>::getValues(const string &filename,
    const int &range) {
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
const vector<T> LinuxParser::GenericParser<T>::getValues(const string &filename)
{
    string line;
    std::ifstream filestream(filename);
    T value;
    vector<T> collector{};
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        while(linestream >> value) collector.emplace_back(value);
        return collector;
    }
    return collector;
}



string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == kOSName) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}


string LinuxParser::Kernel() {
  return GenericParser<string>::getPosition(kProcDirectory + kVersionFilename,
      kKernalItemNumber);
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
float LinuxParser::calculateMemoryUtilization(const vector<float> &meminfo) {
    return (meminfo[kMemTotal_] - (meminfo[kMemFree_] + meminfo[kBuffers_]
    + meminfo[kCached_])) / meminfo[kMemTotal_];
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  return calculateMemoryUtilization(GenericParser<float>::getValues(kProcDirectory
      + kMeminfoFilename, kMemInfoRangeToRead));
}


// Read and return the system Uptime
long LinuxParser::UpTime() {
  return GenericParser<float>::getPosition(kProcDirectory + kUptimeFilename,
      kUptimeItemNumber);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    return GenericParser<int>::getValue(kProcDirectory + kStatFilename,
      kTotalProcess);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    return GenericParser<int>::getValue(kProcDirectory + kStatFilename,
      kRunningProcess);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  return GenericParser<string>::getPosition(kProcDirectory + to_string(pid)
  + kCmdlineFilename, kCmdlineItemNumber);
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  return LinuxParser::UpTime() - stol(ProcessStat(pid, kStartTimePosition))
  / sysconf(_SC_CLK_TCK);
}

// Helper function to collect /proc/[pid]/stat into a vector
string LinuxParser::ProcessStat(int pid, const int &position) {
  return GenericParser<string>::getPosition(kProcDirectory + to_string(pid)
  + kStatFilename, position);
}
