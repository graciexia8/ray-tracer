#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "material.h"

class Sphere : public hittable {
    private:
        point3 center;
        double radius;
        shared_ptr<material> mat;
    public:
        Sphere(point3 center, double radius, shared_ptr<material> mat ) : center(center), radius(radius), mat(mat) {}

        bool hit (const ray& ray, interval ray_t, hit_record& rec) const override {
                // dot porduct order does not matter
                vec3 oc = ray.origin() - center; // if you factor out the dot product this is eqn u get, no actual meaning
                auto a = dot(ray.direction(), ray.direction());
                auto half_b = dot(oc, ray.direction());
                auto c = dot(oc, oc) - radius*radius;
                auto discriminant = (half_b*half_b) - (a*c);
                // discriminant describes the square root portion of the quadratic eqn
                // if dis = 0 -> one real root, if dis = 2 -> two roots (ray passes through circle twice)
                // if dis < 0, ray does not hit the circle
                if (discriminant < 0) {
                    return false;
                } 
                else {
                    // we do -b - srqt(...) and not -b + sqrt(...) because:
                    // we are already assuming sqrt(...) returns either 1 or 2 real roots
                    // if 1 real root then discriminant = 0, if you plug into eqn, the +/- sqrt doesn't even matter
                    // id 2 real roots the you do -b - sqrt(...) to get the smallest t value because it's the closest hit point between ray and sphere (because there are 2 hit poitns)
                    // the larger t value means that the point is farther along the ray => further hit point
                    auto root = (-half_b - sqrt(discriminant)) / a;
                    if (!ray_t.surrounds(root)) {
                        root = (-half_b + sqrt(discriminant)) / a;
                        if (!ray_t.surrounds(root)) {
                            return false;
                        }
                    }

                    rec.t = root;
                    rec.point = ray.at(rec.t);
                    vec3 outwardNormalUnit = (rec.point - center) / radius;
                    rec.set_frontface(ray, outwardNormalUnit);
                    rec.material_pointer = mat;

                    return true;
                }
        }
};

#endif