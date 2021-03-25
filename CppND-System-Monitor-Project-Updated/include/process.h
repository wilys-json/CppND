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
  void CpuUtilization(long active_ticks, long system_ticks);
  std::string Ram();
  long int UpTime() const;
  bool operator>(Process const& other) const;

 private:
  int pid_;
  float cpu_{0};
  long cached_active_ticks_{0};
  long cached_idle_ticks_{0};
  long cached_system_ticks_{0};
};

#endif
