#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

using std::ostringstream;
using std::setfill;
using std::setw;
using std::string;

string Format::ElapsedTime(long seconds) {
  ostringstream time;
  int secs, mins, hours;

  secs = seconds % SEC_IN_MIN;
  seconds /= SEC_IN_MIN;
  mins = seconds % MIN_IN_HOUR;
  seconds /= MIN_IN_HOUR;
  // Since we only return HH:MM:SS, add back residual hours if hours > 24
  hours = seconds % HOUR_IN_DAY + ((seconds / HOUR_IN_DAY) * HOUR_IN_DAY);

  time << setw(2) << setfill('0') << hours << ":";
  time << setw(2) << setfill('0') << mins << ":";
  time << setw(2) << setfill('0') << secs;

  return time.str();
}
