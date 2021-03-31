#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <unistd.h>

#include "process_interface.h"
#include "processor_interface.h"
#include "system_interface.h"
#define HERTZ sysconf(_SC_CLK_TCK)

#include <fstream>
#include <regex>
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
std::vector<int> Pids();
long SystemUpTime();

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

// Processes
const int kStartTimePosition{22};
const std::vector<int> kCpuUtilInfo{13, 14, 15, 16};

// Base Class to implement System Interface
class SystemParser : public SystemInterface {
 public:
  float calculateMemoryUtilization(const std::vector<float>& meminfo);
  float MemoryUtilization() override;
  long UpTime() override;
  int TotalProcesses() override;
  int RunningProcesses() override;
  std::string OperatingSystem() override;
  std::string Kernel() override;
};

// Base Class to implement Processor Interface
class ProcessorParser : public ProcessorInterface {
 public:
  std::vector<long> Utilizations();
  long Jiffies();
  long ActiveJiffies();
  long IdleJiffies();
  float Utilization() override;

 private:
  long prev_active_jiffies_{0};
  long prev_idle_jiffies_{0};
};

// Base Class to implement Process Interface
class ProcessParser : public ProcessInterface {
 public:
  ProcessParser(const int& pid);
  int Pid() const override;
  std::string Command() const override;
  std::string Ram() override;
  std::string Uid() const;
  std::string User() const override;
  long ActiveJiffies() const;
  long int UpTime() const override;
  float CpuUtilization() const override;

 private:
  int pid_;
  std::string command_;
  std::string user_;
  float cpu_{0};
};

};  // namespace LinuxParser

#endif
