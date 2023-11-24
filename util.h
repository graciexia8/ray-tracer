#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Random number generator
inline double random_double() {
    // return a real num in [0,1)
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // return a real num in [min,max)
    return min + (max-min)*random_double();
}
// Common Headers

#include "ray.h"
#include "vec3.h"
#include "interval.h"

#endif