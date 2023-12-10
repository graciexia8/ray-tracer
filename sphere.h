#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "material.h"

class Sphere : public hittable {
    private:
        double radius;
        shared_ptr<material> mat;
        bool _isMoving;
        point3 _center1;
        point3 _center2;
        vec3 _center_direction;
        aabb bbox;

        
        point3 center(double time) const {
            // Linearly interpolate from center1 to center2 according to time, where t=0 yields
            // center1, and t=1 yields center2.
            return _center1 + time*_center_direction;
        }
    public:
        // Stationary sphere
        Sphere(point3 center, double radius, shared_ptr<material> mat ) : _center1(center), radius(radius), mat(mat), _isMoving(false) {
            vec3 rvec = vec3(radius, radius, radius);
            // draw out the vector subtraction/addition from the center of the sphere
            bbox = aabb(center - rvec, center + rvec);
        }

        // Moving Sphere
        Sphere(point3 center1, point3 center2, double radius, shared_ptr<material> mat ) : _center1(center1), _center2(center2), radius(radius), mat(mat), _isMoving(true) {
            _center_direction = _center2 - center1;
            vec3 rvec = vec3(radius, radius, radius);
            aabb box1 = aabb(center1 - rvec, center2 + rvec);
            aabb box2 = aabb(center2 - rvec, center2 + rvec);
            bbox = aabb(box1, box2);
        }

        aabb bounding_box() const override {
            return bbox;
        }

        bool hit (const ray& ray, interval ray_t, hit_record& rec) const override {
                // dot porduct order does not matter
                point3 centerfinal = _isMoving ? center(ray.time()) : _center1;
                vec3 oc = ray.origin() - centerfinal; // if you factor out the dot product this is eqn u get, no actual meaning
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
                    vec3 outwardNormalUnit = (rec.point - centerfinal) / radius;
                    rec.set_frontface(ray, outwardNormalUnit);
                    rec.material_pointer = mat;

                    return true;
                }
        }
};

#endif