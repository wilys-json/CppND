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
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  void CpuUtilization(const long& system_jiffies);
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(Process const& other) const;

 private:
  int pid_;
  float cpu_{0};
  long prev_active_jiffies_{0};
  long prev_system_jiffies_{0};
};

#endif
