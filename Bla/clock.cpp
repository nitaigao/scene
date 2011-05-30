#include "clock.h"
#include <iostream>

float Clock::delta() {
  gettimeofday(&_tend,0);
  double t1 = (double)_tstart.tv_sec + (double)_tstart.tv_usec / 1000000;
  double t2 = (double)_tend.tv_sec + (double)_tend.tv_usec / 1000000;
  float deltaTime = float(t2-t1);
  _tstart = _tend;
  if (deltaTime > 1.0f / 6.0f) {
    std::clog << "capping delta" << std::endl;
    return 1.0f / 6.0f;
  }
  return deltaTime;
}