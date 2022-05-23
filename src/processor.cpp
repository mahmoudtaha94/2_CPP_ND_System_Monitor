#include "processor.h"

#include "linux_parser.h"

// Done: Return the aggregate CPU utilization
// This function's idea is from
// "https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux"
float Processor::Utilization() {
  long idled, totald, Total, NonIdle, Idle;

  Total = LinuxParser::Jiffies();
  Idle = LinuxParser::IdleJiffies();
  NonIdle = LinuxParser::ActiveJiffies();

  // differentiate: actual value minus the previous one
  totald = Total - PrevTotal_;
  idled = Idle - PrevIdle_;

  // update prev values
  PrevTotal_ = Total;
  PrevIdle_ = Idle;
  PrevNonIdle_ = NonIdle;
  return ((float)totald - (float)idled) / (float)totald;
}