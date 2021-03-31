#ifndef SYSTEM_INTERFACE_H
#define SYSTEM_INTERFACE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class SystemInterface {
 public:
  virtual ~SystemInterface(){};
  virtual float MemoryUtilization() = 0;
  virtual long UpTime() = 0;
  virtual int TotalProcesses() = 0;
  virtual int RunningProcesses() = 0;
  virtual string Kernel() = 0;
  virtual string OperatingSystem() = 0;
};

#endif
