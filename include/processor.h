#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // Done: See src/processor.cpp

  // Done: Declare any necessary private members
 private:
 long PrevTotal_{0};
 long PrevIdle_{0};
 long PrevNonIdle_{0};
};

#endif