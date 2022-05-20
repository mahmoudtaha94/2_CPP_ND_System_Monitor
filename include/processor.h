#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // Done: See src/processor.cpp

  // Done: Declare any necessary private members
 private:
 long previdle_{0};
 long previowait_{0};
 long prevuser_{0};
 long prevnice_{0};
 long prevsteal_{0};
 long prevsoftirq_{0};
 long previrq_{0};
 long prevsystem_{0};
};

#endif