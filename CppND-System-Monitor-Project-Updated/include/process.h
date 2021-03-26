// #ifndef PROCESS_H
// #define PROCESS_H
//
// #include <string>
// /*
// Basic class for Process representation
// It contains relevant attributes as shown below
// */
// class Process {
//  public:
//   Process(const int& pid);
//   int Pid() const;
//   std::string User();
//   std::string Command();
//   float CpuUtilization() const;
//   void CpuUtilization(long active_jiffies, long system_jiffies);
//   std::string Ram();
//   long int UpTime() const;
//   bool operator<(Process const& other) const;
//
//  private:
//   int pid_;
//   float cpu_{0};
//   long prev_active_jiffies_{0};
//   long prev_system_jiffies_{0};
// };
//
// #endif

#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid() const;               // DONE: See src/process.cpp
  std::string User() const;      // DONE: See src/process.cpp
  std::string Command() const;   // DONE: See src/process.cpp
  float CpuUtilization() const;  // DONE: See src/process.cpp
  void CpuUtilization(long active_ticks, long system_ticks);
  std::string Ram() const;                 // DONE: See src/process.cpp
  long int UpTime() const;                 // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp
  bool operator>(Process const& a) const;  // DONE: See src/process.cpp

  // DONE: Declare any necessary private members
 private:
  int pid_;
  float cpu_{0};
  long cached_active_ticks_{0};
  long cached_idle_ticks_{0};
  long cached_system_ticks_{0};
};

#endif
