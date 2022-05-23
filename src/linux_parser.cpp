#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
static int num_of_running_processes_ = 0;
static int num_of_processes_ = 0;
static vector<string> cpu_stats_{"", "", "", "", "", "", "", "", "", ""};

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key, line;
  long int value, memtotal, memfree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        memtotal = value;
      } else if (key == "MemFree:") {
        memfree = value;
        break;
      }
    }
  }
  return (((float)memtotal - (float)memfree) / (float)memtotal);
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  LinuxParser::CpuUtilization();
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return std::stoi(cpu_stats_[LinuxParser::kUser_]) +
         std::stoi(cpu_stats_[LinuxParser::kNice_]) +
         std::stoi(cpu_stats_[LinuxParser::kSystem_]) +
         std::stoi(cpu_stats_[LinuxParser::kIRQ_]) +
         std::stoi(cpu_stats_[LinuxParser::kSoftIRQ_]) +
         std::stoi(cpu_stats_[LinuxParser::kSteal_]);
}

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  return std::stoi(cpu_stats_[LinuxParser::kIdle_]) +
         std::stoi(cpu_stats_[LinuxParser::kIOwait_]);
}

// Done: Read and return CPU utilization
void LinuxParser::CpuUtilization() {
  string line, cpu, stat;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    //    user    nice   system  idle      iowait irq   softirq  steal  guest
    //    guest_nice
    // cpu  74608   2520   24433   1117073   6176   4054  0        0      0 0
    linestream >> cpu >> cpu_stats_[LinuxParser::kUser_] >>
        cpu_stats_[LinuxParser::kNice_] >> cpu_stats_[LinuxParser::kSystem_] >>
        cpu_stats_[LinuxParser::kIdle_] >> cpu_stats_[LinuxParser::kIOwait_] >>
        cpu_stats_[LinuxParser::kIRQ_] >> cpu_stats_[LinuxParser::kSoftIRQ_] >>
        cpu_stats_[LinuxParser::kSteal_] >> cpu_stats_[LinuxParser::kGuest_] >>
        cpu_stats_[LinuxParser::kGuestNice_];
    // Since we're in this file, let's get the number of processess and active
    // processes
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        num_of_processes_ = value;
      } else if (key == "procs_running") {
        num_of_running_processes_ = value;
        break;
      }
    }
  }
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return num_of_processes_; }

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return num_of_running_processes_; }

// Done: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// Done: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string key, line;
  int value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:") {
        linestream >> value;
        break;
      }
    }
  }
  return to_string(value);
}

// Done: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string key, line;
  int value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") {
        linestream >> value;
        break;
      }
    }
  }
  return to_string(value);
}

// Done: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, name, x, uid, token;
  string delimiter = ":";
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      int pos = 0;
      for (int i = 0; i < 3; i++) {
        pos = line.find(delimiter);
        token = line.substr(0, pos);
        if (i == 0) name = token;
        if (i == 1) x = token;
        if (i == 2) uid = token;
      }
      if (uid == LinuxParser::Uid(pid)) break;
    }
  }
  return name;
}

// Done: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, out;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i <= 21; i++) {
      linestream >> out;
    }
  }
  filestream.close();
  return std::stoi(out);
}

// Done: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, utime, stime, cutime, cstime, dummy;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i <= 13; i++) {
      if (i == 13)
        linestream >> utime >> stime >> cutime >> cstime;
      else
        linestream >> dummy;
    }
  }
  filestream.close();
  return std::stoi(utime) + std::stoi(stime) + std::stoi(cutime) +
         std::stoi(cstime);
}
