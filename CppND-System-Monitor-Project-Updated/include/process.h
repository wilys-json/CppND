#ifndef PROCESS_H
#define PROCESS_H

#include "detect_os.h"
#include <algorithm>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process : public ProcessParser {
 public:
  Process(const int& pid);
  bool operator<(Process const& other) const;
};

#endif
