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
  std::string Ram();
  long int UpTime() const;
  bool operator<(Process const& other) const;

 private:
  int pid_;
  std::string command_;
  std::string user_;
  float cpu_{0};
  long prev_jiffies_{0};
  long prev_uptime_{0};
};

#endif
