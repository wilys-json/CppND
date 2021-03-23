#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <vector>


// Process macro
#define STARTTIME_IDX 21

namespace LinuxParser {


// Generic Key-Value / Value Parser
template <typename T>
class GenericParser {
public:
    static const T ParseLine(const std::string&, const int&);
    static const T Parse(const std::string&, const std::string&);
    static const std::vector<T> Parse(const std::string&, const int&);
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
const int kMemInfoRangeToRead{5};
enum MemInfo {
  kMemTotal_ = 0,
  kMemFree_,
  kMemAvailable_,
  kBuffers_,
  kCached_
};

float calculateMemoryUtilization(std::vector<float> meminfo);
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
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
std::vector<std::string> ProcessStat(int pid); // helper function
};  // namespace LinuxParser

#endif
