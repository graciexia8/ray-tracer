#ifndef HITTABLE_H
#define HITTABLE_H
#include "ray.h"
#include "vec3.h"

class hit_record {
    public:
        double t;
        point3 point;
        vec3 normal;
        bool frontface;

        void set_frontface(const ray& ray, vec3 outwardNormal) {
            frontface = (dot(ray.direction(), outwardNormal) < 0);
            normal = frontface ? outwardNormal : -outwardNormal;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;
        virtual bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};
#endif