#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include "integrators.hpp"
#include <iostream>
#include <iomanip>
#include <functional>
#include <stdexcept>
#include <vector> 
#include <limits> 
#include <fstream> 
#include <sstream> 
#include <string> 
#include <cstdio> 
#include <iostream> 

using namespace std; 

// Note: this code is not safe wrt the number of intervals
// Make sure the number of intervals used is valid for each integrator


//_______________________________________________________________________________________________________________
// Integration using trapezoid rule 
// npts : number of points used in calculation (npts>=2)
double trapez(function<double(double)> fcn, unsigned int npts, double x_min, double x_max) {

  if (npts < 2) {
    throw invalid_argument("in <trapez>: number of pts must be at least 2"); 
    return std::numeric_limits<double>::quiet_NaN();
  }

  double sum=0.;		 

  //get list of fcn evaluation points
  const double dx = (x_max - x_min)/((double)npts-1); 

  //compute the integral according to simpson's rule 
  double x = x_min; 

  for (int i=0; i<npts; i++) { 

    //compute the weight, according to the trapezoid rule
    double weight = (i == 0 || i == npts-1) ? dx/2. : dx; 
  
    sum += weight * fcn(x); 
    x   += dx; 
  }

  return sum;
}      

//_______________________________________________________________________________________________________________
// Integration using Simpson's rule
// npts : number of points used in calculation (npts odd, and >=3)
double simpson(function<double(double)> fcn, unsigned int npts, double x_min, double x_max){  
  
  //check to make sure that the 'npts' is >= 3, and odd. 
  if (npts < 3 || npts % 2 != 1) {
    throw invalid_argument("in <simpson>: Number of points invalid. must be >= 3, and odd."); 
    return std::numeric_limits<double>::quiet_NaN(); 
  }
  double sum=0.;

  //This is the result of fitting a parabola to each sub-interval of 3 points, and integrating that parabola. 
  double x=x_min; 

  const double dx = (x_max - x_min)/((double)npts - 1); 

  for (int i=0; i<npts; i++) {

    double weight; 
    //find out which weight to use
    if (i == 0 || i == npts-1) {

      //endpoints have the weight of dx/3
      weight = dx*(1./3.); 
    } else {

      //even points {2,4,6,...} have 4.dx/3, odd points {1,3,5,...} have 2.dx/3
      if (i % 2 == 1) { weight = dx*(4./3.); } else { weight = dx*(2./3.); }
    }

    sum += weight * fcn(x); 
    x   += dx; 
  } 

  return sum;
}  


//_______________________________________________________________________________________________________________
GaussInt::GaussInt(const char* path_dbfile) 
  : is_init(false), fPoints{}
{
  //see if we can open the file
  ifstream file(path_dbfile);

  if (!file.is_open()) {
    ostringstream oss; 
    oss << "in <GaussInt::GaussInt>: unable to open file '" << path_dbfile << "'";
    throw invalid_argument(oss.str()); 
    return;  
  }

  //now, we can parse the file 
  string line, token; 
  
  //start with an empty vector 
  fPoints.push_back({});
  
  vector<GaussQuadPoint_t> new_order{}; 
  int npts_order=1;
  
  unsigned int line_num = 0; 

  while(getline(file,line)) { line_num++; 

    //check if line is empty, or if this is a comment line, starting with '#', then skip it. 
    if (line.empty() || line[0]=='#') {
      //check to see if this newline was encountered before we're done parsing all our points for this order
      if (!new_order.empty()) {
        ostringstream oss; 
        oss <<  "in <GaussInt::GaussInt>: blank line or comment line (#) encountered before expected."
                " parsing order " << npts_order << ", line " << line_num << " size=" << new_order.size(); 
        throw invalid_argument(oss.str()); 
        return; 
      }
      continue; 
    }

    //if not, we're ready to parse the data
    istringstream iss(line); 
    
    //add this point to the list of points for this order 
    double x, weight; 
    iss >> x >> weight; 
    new_order.push_back({ x, weight }); 

    //if we have enough points for this order, then move on to the next one.
    //for example, 1st order has 1 total points, 2nd order has 2, etc.  
    if (new_order.size() >= npts_order) {

      fPoints.push_back( new_order ); 
      new_order.clear();
      npts_order++;  
    }
  }

  //set the init status flag to 'true' 
  is_init=true; 
}


