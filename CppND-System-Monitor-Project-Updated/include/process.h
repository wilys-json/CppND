#ifndef PROCESS_H
#define PROCESS_H

#include "detect_os.h"

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process : public ProcessParser {
 public:
  Process(const int& pid);
  float CpuUtilization() const;
  bool operator<(Process const& other) const;

 private:
  std::string command_;
  std::string user_;
  float cpu_{0};
  long prev_jiffies_{0};
  long prev_uptime_{0};
};

#endif
