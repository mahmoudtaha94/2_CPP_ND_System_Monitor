#include "format.h"

#include <string>

using std::string;

// Done: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  std::string Hours = std::to_string(seconds / 3600);
  std::string Mins = std::to_string((seconds % 3600) / 60);
  std::string Secs = std::to_string(seconds % 60);
  Hours.insert(0, 2 - Hours.length(), '0');
  Mins.insert(0, 2 - Mins.length(), '0');
  Secs.insert(0, 2 - Secs.length(), '0');
  return Hours + ":" + Mins + ":" + Secs;
}