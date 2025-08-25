#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

#include "color.h"
#include "ray.h"
#include "vec3.h"

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.14159265358979;

inline double degrees_to_radians(double degrees) {
    return degrees * (pi / 180.0);
}

#endif UTILS_H