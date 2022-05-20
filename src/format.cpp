#include <string>

#include "format.h"

using std::string;

// Done: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  int hours = seconds/3600;
  int mins = (seconds%3600)/60;
  int secs = seconds%60;
  return std::to_string(hours)+":"+std::to_string(mins)+":"+std::to_string(secs); 
}