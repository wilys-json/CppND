#include "processor.h"
#include "linux_parser.h"
#include <iostream>
// Return the aggregate CPU utilization
float Processor::Utilization() {
  float utilization;
  long activeJiffies = LinuxParser::ActiveJiffies();
  long idleJiffies = LinuxParser::IdleJiffies();

  utilization = (activeJiffies - prev_active_jiffies_)
  / (((activeJiffies - prev_active_jiffies_)
      + (idleJiffies - prev_idle_jiffies_)) * 1.0);

  prev_active_jiffies_ = activeJiffies;
  prev_idle_jiffies_ = idleJiffies;

  return utilization;
}
