#ifndef DIELECTRICS_H
#define DIELECTRICS_H

#include "material.h"
#include "hittable.h"

class dielectric : public material {
  public:
    dielectric(double index_of_refraction) : index_of_refraction(index_of_refraction) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
    const override {
        attenuation = color(1.0, 1.0, 1.0);
        double index_of_refraction_air = 1.0;
        auto eta_over_eta_prime = rec.frontface ? index_of_refraction_air / index_of_refraction : index_of_refraction / index_of_refraction_air;
        // have to normalize r_in in order to satisfy the dot product of two unit vectors is just cos theta
        // explained in the proof
        vec3 r_in_direction_unit = unit_vector(r_in.direction());

        double cos_theta = fmin(dot(-r_in_direction_unit, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = index_of_refraction * sin_theta > 1.0;
        if (cannot_refract || reflectance(cos_theta, eta_over_eta_prime) > random_double()) {
            // reflect
            vec3 reflected_direction = reflect(r_in_direction_unit, rec.normal);
            scattered = ray(rec.point, reflected_direction, r_in.time());
        }
        else {
            // rec is already normalized
            vec3 refracted_direction = refract(r_in_direction_unit, rec.normal, eta_over_eta_prime);
            scattered = ray(rec.point, refracted_direction, r_in.time());
        }

        return true;
    }

  private:
    double index_of_refraction;
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};

#endif