#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <set>

#include "detect_os.h"
#include "process.h"
#include "processor.h"

class System : public SystemParser {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();

 private:
  Processor cpu_;
  std::vector<Process> processes_{};
};

#endif
