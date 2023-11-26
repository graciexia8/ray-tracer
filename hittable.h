#ifndef HITTABLE_H
#define HITTABLE_H
#include "util.h"

class material;

class hit_record {
    public:
        double t;
        point3 point;
        vec3 normal;
        shared_ptr<material> material_pointer;
        bool frontface;

        void set_frontface(const ray& ray, vec3 outwardNormal) {
            frontface = (dot(ray.direction(), outwardNormal) < 0);
            normal = frontface ? outwardNormal : -outwardNormal;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};
#endif