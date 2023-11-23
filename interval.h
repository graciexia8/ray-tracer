#ifndef INTERVAL_H
#define INTERVAL_H
#include "util.h"
class interval {
    public:
        double min, max;

        interval() : min(-infinity), max(+infinity) {}
        interval(double min, double max) : min(min), max(max) {}

        bool contains(double value) {
            return value >= min && value <= max;
        }

        bool surrounds(double value) {
            return value > min && value < max;
        }

        double clamp(double value) {
            if (value < min) return min;
            if (value > max) return max;
            return value;
        }

        static const interval empty;
        static const interval universe;
};

const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);

#endif