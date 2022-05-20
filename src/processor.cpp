#include <string>
#include <vector>
#include "processor.h"
#include "linux_parser.h"
#include <iostream>

// Done: Return the aggregate CPU utilization
float Processor::Utilization() {
  //    user    nice   system  idle      iowait irq   softirq  steal  guest  guest_nice
  //cpu  74608   2520   24433   1117073   6176   4054  0        0      0      0
  long idled, totald, Total, PrevTotal, NonIdle, PrevNonIdle, Idle, PrevIdle;
  long user, nice, system, idle, iowait, irq, softirq, steal;
  std::vector<std::string> cpu_stats;
  cpu_stats = LinuxParser::CpuUtilization();

  user = std::stoi(cpu_stats[LinuxParser::kUser_]);
  nice = std::stoi(cpu_stats[LinuxParser::kNice_]);
  system = std::stoi(cpu_stats[LinuxParser::kSystem_]);
  idle = std::stoi(cpu_stats[LinuxParser::kIdle_]);
  iowait = std::stoi(cpu_stats[LinuxParser::kIOwait_]);
  irq = std::stoi(cpu_stats[LinuxParser::kIRQ_]);
  softirq = std::stoi(cpu_stats[LinuxParser::kSoftIRQ_]);
  steal = std::stoi(cpu_stats[LinuxParser::kSteal_]);
  
  PrevIdle = previdle_ + previowait_;
  Idle = idle + iowait;

  PrevNonIdle = prevuser_ + prevnice_ + prevsystem_ + previrq_ + prevsoftirq_ + prevsteal_;
  NonIdle = user + nice + system + irq + softirq + steal;

  PrevTotal = PrevIdle + PrevNonIdle;
  Total = Idle + NonIdle;
  
  //save previous values
  previdle_ = idle;
  previowait_ = iowait;
  prevuser_ = user;
  prevnice_ = nice;
  prevsystem_ = system;
  previrq_ = irq;
  prevsoftirq_ = softirq;
  prevsteal_ = steal;

  //differentiate: actual value minus the previous one
  totald = Total - PrevTotal;
  idled = Idle - PrevIdle;

  return ((float)totald - (float)idled)/(float)totald; 
}