#ifndef CLOCK_H
#define CLOCK_H

#include <sys/time.h>

class Clock
{
  
public:
  
  ~Clock() { };
  
  Clock() : first_pass_(true) { };
  
  float delta();
  
private:
  
  Clock(const Clock & copy) { };
  Clock & operator = (const Clock & copy) { return *this; };
  
  struct timeval _tstart, _tend;
  bool first_pass_;
  
};

#endif