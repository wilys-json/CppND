#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;
using std::runtime_error;


template <typename T>
const T LinuxParser::GenericParser<T>::ParseLine(const string &filename,
    const int &itemNumber) {
    string line;
    std::ifstream filestream(filename);
    T value;
    try {
        if (filestream.is_open()) {
            std::getline(filestream, line);
            std::istringstream linestream(line);
            for (int i = 0; i < itemNumber; i++) {
                linestream >> value;
            }
            return value;
        } else {
            throw runtime_error("Cannot open file: " + filename);
        }
    } catch (...) {
        throw runtime_error("Cannot open file: " + filename);
    }
}


template <typename T>
const T LinuxParser::GenericParser<T>::Parse(const string &filename,
    const string& targetKey) {
    string line, key;
    std::ifstream filestream(filename);
    T value;
    try {
        if (filestream.is_open()) {
            while (std::getline(filestream, line)) {
                std::istringstream linestream(line);
                while (linestream >> key >> value) {
                            if(key == targetKey) return value;
                }
            }
        } else {
            throw runtime_error("Cannot open file: " + filename);
        }
    } catch (...) {
        throw runtime_error("Cannot open file: " + filename);
    }

}


template <typename T>
const vector<T> LinuxParser::GenericParser<T>::Parse(const string &filename,
    const int &range) {
    string line, key;
    std::ifstream filestream(filename);
    T value;
    vector<T> collector;
    try {
        if (filestream.is_open()) {
        for (int i = 0; i < range; i++) {
            std::getline(filestream, line);
            std::istringstream linestream(line);
            linestream >> key >> value;
            collector.emplace_back(value);
        }
          return collector;
      } else {
          throw runtime_error("Cannot open file: " + filename);
      }
    } catch (...) {
        throw runtime_error("Cannot open file: " + filename);
    }

}


// DONE: An example of how to read data from the filesystem
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
  // string os, version, kernel;
  // string line;
  // std::ifstream stream(kProcDirectory + kVersionFilename);
  // if (stream.is_open()) {
  //   std::getline(stream, line);
  //   std::istringstream linestream(line);
  //   linestream >> os >> version >> kernel;
  // }
  // return kernel;
  return LinuxParser::GenericParser<string>::ParseLine(kProcDirectory + kVersionFilename,
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
float LinuxParser::calculateMemoryUtilization(vector<float> meminfo) {
    return (meminfo[kMemTotal_] - (meminfo[kMemFree_] + meminfo[kBuffers_]
    + meminfo[kCached_])) / meminfo[kMemTotal_];
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  return calculateMemoryUtilization(GenericParser<float>::Parse(kProcDirectory
      + kMeminfoFilename, kMemInfoRangeToRead));
}


// Read and return the system Uptime
long LinuxParser::UpTime() {
  // string line;
  // long uptime{0};
  // std::ifstream system_filestream(kProcDirectory + kUptimeFilename);
  // if(system_filestream.is_open()) {
  //   std::getline(system_filestream, line);
  //   std::istringstream linestream(line);
  //   linestream >> uptime;
  //   return uptime;
  // }
  // return uptime;
  return LinuxParser::GenericParser<float>::ParseLine(kProcDirectory + kUptimeFilename,
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
    return GenericParser<int>::Parse(kProcDirectory + kStatFilename, kTotalProcess);
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    return GenericParser<int>::Parse(kProcDirectory + kStatFilename, kRunningProcess);
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmd{};
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cmd;
    return cmd;
  }
  return cmd;
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
  return LinuxParser::UpTime() - (stol(ProcessStat(pid)[STARTTIME_IDX]) / sysconf(_SC_CLK_TCK));
}

// Helper function to collect /proc/[pid]/stat into a vector
vector<string> LinuxParser::ProcessStat(int pid) {

  vector<string> stats{};
  string stat, line;
  std::ifstream statfile_stream(kProcDirectory + to_string(pid) + kStatFilename);

  if (statfile_stream.is_open()) {
      std::getline(statfile_stream, line);
      std::istringstream linestream(line);
      while(linestream >> stat) stats.push_back(stat);
      return stats;
    }
  return stats; // return /proc/[pid]/stat as a vector
}
