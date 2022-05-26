#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <experimental/filesystem>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
namespace fs = std::experimental::filesystem;

// Done: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Done: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();
  for (const auto& entry :
       fs::directory_iterator(LinuxParser::kProcDirectory)) {
    try {
      string proc_path = entry.path();
      int process_dir = std::stoi(proc_path.substr(6, -1));
      Process proc;
      proc.SetPid(process_dir);
      if(proc.User() != "" && proc.Command() != ""){
        processes_.push_back(proc);
      }
    } catch (...) {
      continue;
    }
  }
  return processes_;
}

// Done: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Done: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Done: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Done: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Done: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Done: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }