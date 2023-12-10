#ifndef AABB_H
#define AABB_H
#include "interval.h"

class aabb {
    public:
        interval x, y, z;
        aabb() {}
        aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) {}

        aabb(const point3& base_lower_left, const point3& top_upper_right) {
            x = interval(fmin(base_lower_left[0], top_upper_right[0]), fmax(base_lower_left[0], top_upper_right[0]));
            y = interval(fmin(base_lower_left[1], top_upper_right[1]), fmax(base_lower_left[1], top_upper_right[1]));
            z = interval(fmin(base_lower_left[2], top_upper_right[2]), fmax(base_lower_left[2], top_upper_right[2]));
        }

        aabb(const aabb& box1, const aabb& box2) {
            x = interval(box1.x, box2.x);
            y = interval(box1.y, box2.y);
            z = interval(box1.z, box2.z);
        }

        const interval& axis(int num) const {
            if (num == 0) {return x;}
            if (num == 1) {return y;}
            if (num == 2) {return z;}
        }

        bool hit(const ray& r, interval ray_t) {
            for (int a = 0; a < 3; a++) {
                // get bx, by, bz depending on loop iteration
                // do inversion so we don't need to divide multiple times
                auto invD = 1 / r.direction()[a];
                auto t0 = axis(a).min - r.origin()[a]*invD;
                auto t1 = axis(a).max - r.origin()[a]*invD;

                // since invD is the direction indicator, if it is negative
                // it indicates that the ray was travelling in reverse direction
                // Because we specified that 0 -> positive values is a positive direction??
                if (invD < 0) {
                    // Swap values so that t0 is always smalled than t1 and so comes before t1
                    std::swap(t0, t1);
                }

                // Keep track of the portions where there may be overlap
                if (t0 > ray_t.min) { ray_t.min = t0;}
                if (t1 < ray_t.max) {ray_t.max = t1;}

                // Draw it out in 2d space to understand,
                // If this condition is fulfilled then there is no overlap that has occurred
                if (ray_t.max <= ray_t.min) {
                    return false;
                }
            }

            return true;
        }
};

#endif