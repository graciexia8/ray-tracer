#ifndef INTERVAL_H
#define INTERVAL_H
#include "util.h"
class interval {
    public:
        double min, max;

        interval() : min(-infinity), max(+infinity) {}
        interval(double min, double max) : min(min), max(max) {}
        interval(const interval& a, const interval& b) : min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}

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

        interval expand(double delta) {
            auto padding = delta / 2;
            return interval(min-padding, max+padding);
        }

        static const interval empty;
        static const interval universe;
};

const static interval empty(+infinity, -infinity);
const static interval universe(-infinity, +infinity);

#endif