#ifndef METAL_H
#define METAL_H

#include "material.h"
#include "hittable.h"

class metal : public material {
  public:
    metal(const color& a, double fuzz) : albedo(a), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.point, reflected+fuzz*random_unit_vector());
        attenuation = albedo;
        return (dot(rec.normal, scattered.direction()) > 0);
    }

  private:
    color albedo;
    double fuzz;
};

#endif