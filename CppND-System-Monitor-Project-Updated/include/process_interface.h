#ifndef PROCESS_INTERFACE_H
#define PROCESS_INTERFACE_H

#include <string>

using std::string;

class ProcessInterface {
  public:
    virtual ~ProcessInterface() {};
    virtual int Pid() const = 0;
    virtual string User() const = 0;
    virtual string Command() const = 0;
    virtual string Ram() = 0;
    virtual long int UpTime() const = 0;
    virtual float CpuUtilization() const = 0;
};

#endif
