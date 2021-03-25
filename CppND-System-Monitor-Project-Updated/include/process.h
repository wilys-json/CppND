#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(const int& pid);
  int Pid() const;
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  void CpuUtilization(const long& active_jiff, const long& system_jiff);
  std::string Ram();
  long int UpTime() const;
  bool operator<(Process const& other) const;

 private:
  int pid_;
  float cpuUtil_{0};
  long prev_active_jiffies_{0};
  long prev_system_jiffies_{0};
};

#endif
