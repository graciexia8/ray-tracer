#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"
#include "hittable.h"

class lambertian : public material {
        color albedo;
    public:
        lambertian(const color& albedo) : albedo(albedo) {}
        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 direction = rec.normal + random_unit_vector();

            if (direction.near_zero()) {
                direction = rec.normal;
            }
            scattered = ray(rec.point, direction, r_in.time());
            attenuation = albedo;

            return true;
        }

};

#endif