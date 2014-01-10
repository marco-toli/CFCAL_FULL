#ifndef LedFiberTiming_H
#define LedFiberTiming_H 1

#include <iostream>
#include <cmath>

#include "TVector3.h"



struct Fiber{
  double   lengthTotal;
  double   refrIndCore;   
  double   refrIndClad;   
  double   refrIndAir;    
  double   refrIndDet;    
  double   attenuation;
  TVector3 position; 
  TVector3 direction;
  double   radius;
};

struct Photon{
  TVector3 position;
  TVector3 direction;
  double   dist;   // relative from 0 to 1 (0 & 1 = photodetector)
};

struct Travel{
  int      nmax;
  double   prob[10];
  double   time[10];
};

Fiber FiberInit(const double& length, const double& radius, const double& att,
                const bool& isVertical);

double fresnelReflection(const double& th, const double& R);

Travel GetTimeAndProbability(Photon& ph, const Fiber* fib, const double& prodTime);

#endif
