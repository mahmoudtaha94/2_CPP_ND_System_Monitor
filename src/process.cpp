#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Done: Return this process's ID
int Process::Pid() { return pid_; }

// Done: Return this process's CPU utilization
float Process::CpuUtilization() {
  long active_time, time_since_process_started;
  active_time = LinuxParser::ActiveJiffies(pid_) / sysconf(_SC_CLK_TCK);
  time_since_process_started = Process::UpTime();
  if(active_time == 0 || time_since_process_started==0){
    cpu_util_ = 0;
  }else{
    cpu_util_ = (float)active_time / (float)time_since_process_started;
  }
  return cpu_util_;
}

// Done: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Done: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Done: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() {
  uptime_ = LinuxParser::UpTime() - (LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK));
  return uptime_;
}

// Done: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return this->cpu_util_ < a.GetCpuUtilization();
}

void Process::SetPid(int id) { pid_ = id; }
float Process::GetCpuUtilization() const { return cpu_util_; }