#ifndef SYSTEM_H
#define SYSTEM_H

#include <vector>

#include "detect_os.h"
#include "process.h"
#include "processor.h"

// System representation
class System : public SystemParser {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();

 private:
  Processor cpu_;
  std::vector<Process> processes_ = {};
};

#endif
