#ifndef INTEGRATORS_HPP
#define INTEGRATORS_HPP

#include <vector>
#include <features.h>
#include <functional> 

// Integration using trapezoid rule
double trapez  (std::function<double(double)> fcn, unsigned npts, double x_min, double x_max);

// Integration using Simpson's rule 
double simpson (std::function<double(double)> fcn, unsigned npts, double x_min, double x_max);

// This struct represents a single gauss quad point, with its respective weight. 
struct GaussQuadPoint_t {
#if __GNUC_PREREQ(4,6)
  __float128 x, weight; 
#else 
  double x, weight; 
#endif 
};

class GaussInt {
public: 
  //public constructor
  GaussInt(const char* path_dbfile=""); 
  ~GaussInt() { fPoints.clear(); }

  double Integral(std::function<double(double)> fcn, unsigned int npts, double x_min, double x_max) const; 
private: 
  bool is_init=false; 
  std::vector<std::vector<GaussQuadPoint_t>> fPoints; 
}; 

#endif


