#ifndef FORMAT_H
#define FORMAT_H

#include <string>

#define SEC_IN_MIN 60
#define MIN_IN_HOUR 60
#define HOUR_IN_DAY 24

namespace Format {
std::string ElapsedTime(long times);  
};                                    // namespace Format

#endif
