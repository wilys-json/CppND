#ifndef SYSTEM_H
#define SYSTEM_H

#include "process.h"
#include "processor.h"
#include "detect_os.h"

#include <vector>


class System : public SystemParser {
  public:
  Processor& Cpu();
  std::vector<Process>& Processes();

 private:
   Processor cpu_;
   std::vector<Process> processes_ = {};
};

#endif
