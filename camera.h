#ifndef CAMERA_H
#define CAMERA_H
#include "util.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

class camera {
    public:
        
        double aspect_ratio = 1.0;
        int image_width = 100;
        int samples_per_pixel = 10;
        int max_depth = 10;

        void render(const hittable_list& world) {
            init();
            // Render
            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            // for (int i = 0 ; i < image_width; i++) {
            for (int j = 0; j < image_height; ++j) {

                // for (int j = 0; j < image_height; j++) {
                for (int i = 0; i < image_width; ++i) {
                    auto pixel_center = vec3(static_cast<double>(P00.x()+(i*delta_u.x())),
                        static_cast<double>(P00.y()+(j*delta_v.y())), 
                        static_cast<double>(P00.z())
                    );
                    color pixel_color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray sampled_ray = get_ray(i,j);
                        pixel_color += ray_color(sampled_ray, world, max_depth);

                    }
                    // // auto pixel_center = P00 + (i * delta_u) + (j * delta_v);
                    // auto ray_direction = pixel_center - camera_center; // direction of ray is from camera to pixel
                    // ray r(camera_center, ray_direction);

                    // color pixel_color = ray_color(r, world);
                    // auto pixel_color = color(double(j) / image_width, double(i) /image_height, 0);
                    write_color(std::cout, pixel_color, samples_per_pixel);
                }
            } 
            std::clog << "Done.\n";
        }

    private:
        int image_height;
        point3 camera_center;
        point3 P00;
        vec3 delta_u;
        vec3 delta_v;

        void init() {
            image_height = static_cast<int>(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            // // Viewport & Camera
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
            // // auto viewport_width = viewport_height*(static_cast<double>(image_width/image_height));
            auto focal_length = 1.0;
            camera_center = point3(0,0,0);


            // // vecotrs that define the viewport width and height
            auto viewport_u = vec3(viewport_width, 0, 0);
            auto viewport_v = vec3(0, -viewport_height, 0);
            delta_u = viewport_u / image_width;
            delta_v = viewport_v / image_height; // Debug #1 had a typo

            // // upper left starting pixel
            auto Q = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;

            P00 = vec3(Q.x()+(static_cast<double>(delta_u.x()/2.0)),
            static_cast<double>(Q.y()+(delta_v.y()/2.0)),
                static_cast<double>(Q.z()));
            // auto P00 = Q + 0.5 * (delta_u + delta_v);
        }

        color ray_color(const ray& r, const hittable& world, int max_depth) const {
            if (max_depth <= 0) {
                return color(0,0,0);
            }

            hit_record rec;
            if (world.hit(r, interval(0, infinity), rec)) {
                vec3 direction = rec.normal + random_unit_vector();
                return 0.5 * ray_color(ray(rec.point, direction), world, max_depth-1);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0); // get a value a of a between 0 to 1 based to y coord of ray
            auto white = color(1.0, 1.0, 1.0);
            auto blue = color(0.5, 0.7, 1.0);
            return (1.0-a)*white + a*blue;
        }

        ray get_ray(int i, int j) {
            // Get a randomly sampled camera ray for the pixel at location i,j.
            auto pixel_center = vec3(static_cast<double>(P00.x()+(i*delta_u.x())),
                static_cast<double>(P00.y()+(j*delta_v.y())), 
                static_cast<double>(P00.z())
            );
            auto pixel_sample = pixel_center + pixel_sample_square();
            auto ray_origin = camera_center;
            auto ray_direction = pixel_sample - ray_origin;
            return ray(ray_origin, ray_direction);
        }

        vec3 pixel_sample_square() {
            auto px = -0.5 + random_double();
            auto py = -0.5 + random_double();
            return (px * delta_u) + (py * delta_v);
        }
};
#endif