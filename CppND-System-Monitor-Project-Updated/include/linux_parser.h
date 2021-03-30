#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <unistd.h>
#define HERTZ sysconf(_SC_CLK_TCK)

#include <fstream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

namespace LinuxParser {

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
const std::string kCPUKey("cpu");
const std::string kProcessRAM("VmSize");
const std::string kUID("Uid:");

// System
const int kKernalItemNumber{3};
const int kUptimeItemNumber{1};
const int kMemInfoRangeToRead{5};
enum MemInfo { kMemTotal_ = 0, kMemFree_, kMemAvailable_, kBuffers_, kCached_ };

float calculateMemoryUtilization(const std::vector<float>& meminfo);
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

std::vector<long> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long IdleJiffies();

// Processes
const int kStartTimePosition{22};
const std::vector<int> kCpuUtilInfo{13, 14, 15, 16};

std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long ActiveJiffies(int pid);
long int UpTime(int pid);

};  // namespace LinuxParser

#endif
