#ifndef PROCESS_H
#define PROCESS_H

#include <algorithm>

#include "detect_os.h"

// Process representation
class Process : public ProcessParser {
 public:
  Process(const int& pid);
  bool operator<(Process const& other) const;
};

#endif
