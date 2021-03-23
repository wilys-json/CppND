#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <stdexcept>


namespace LinuxParser {


// Generic Linux File Parser
template <typename T>
struct GenericParser {
  /**
    Function template to get the k-th (position) element from a file.
    @param filename : name of the file
    @param position : target position
    @return : the target value from the file
  */
  static const T getPosition(const std::string &filename, const int &position);

  /**
    Function template to get the value of the target key from a file.
    @param filename : name of the file
    @param targetKey : target key
    @return : the value of the target key
  */
  static const T getValue(const std::string &filename,
    const std::string &targetKey);

  /**
    Function template to collect all values from a file into a vector.
    @param filename : name of the file
    @return : the vector of all values in the file
  */
  static const std::vector<T> getValues(const std::string &filename);

  /**
    Function template to collect values of keys in a defined range.
    @param filename : name of the file
    @param range : number of items to collect
    @return : the vector of values in the defined range
  */
  static const std::vector<T> getValues(const std::string &filename,
    const int &range);
};

// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// Keys
const std::string kTotalProcess{"processes"};
const std::string kRunningProcess{"procs_running"};
const std::string kOSName{"PRETTY_NAME"};

// System
const int kKernalItemNumber{3};
const int kUptimeItemNumber{1};
const int kCmdlineItemNumber{1};
const int kMemInfoRangeToRead{5};
enum MemInfo {
  kMemTotal_ = 0,
  kMemFree_,
  kMemAvailable_,
  kBuffers_,
  kCached_
};

float calculateMemoryUtilization(const std::vector<float> &meminfo);
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes

const int kStartTimePosition{21};

std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
std::string ProcessStat(int, const int&); // helper function


//
};  // namespace LinuxParser

#endif
