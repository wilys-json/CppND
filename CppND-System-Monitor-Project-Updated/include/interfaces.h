#ifndef OBJECT_INTERFACES_H
#define OBJECT_INTERFACES_H

#include <string>

using std::string;

class ProcessInterface {
  public:
    virtual ~ProcessInterface() {};
    virtual int Pid() const;
    virtual string User() const;
    virtual string Command() const;
    virtual float CpuUtilization() const;
    virtual string Ram();
    virtual long int UpTime() const;
    virtual bool operator<(Process const& other) const;
};

class ProcessorInterface {
  public:
    virtual ~ProcessorInterface() {};
    virtual float Utilization();
};

class SystemInterface {
  public:
    virtual ~SystemInterface() {};
    virtual float MemoryUtilization();
    virtual long UpTime();
    virtual int TotalProcesses();
    virtual int RunningProcesses();
    virtual string Kernel();
    virtual string OperatingSystem();
};


#endif
