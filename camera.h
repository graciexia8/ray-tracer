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
        double vfov = 90; // Verticel view angle (field of view)
        point3 lookfrom = point3(0,0,-1);  // Point camera is looking from
        point3 lookat   = point3(0,0,0);   // Point camera is looking at
        vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction

        void render(const hittable_list& world) {
            init();
            // Render
            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; ++j) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;

                for (int i = 0; i < image_width; ++i) {
                    color pixel_color(0,0,0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray sampled_ray = get_ray(i,j);
                        pixel_color += ray_color(sampled_ray, world, max_depth);

                    }

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
        vec3   u, v, w;        // Camera frame basis vectors


        void init() {
            image_height = static_cast<int>(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            // // Viewport & Camera
            camera_center = lookfrom;

            auto focal_length = (lookfrom - lookat).length();
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2 * h * focal_length;
            auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

            // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // // Calculate the vectors across the horizontal and down the vertical viewport edges.
            // auto viewport_u = vec3(viewport_width, 0, 0);
            // auto viewport_v = vec3(0, -viewport_height, 0);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
            vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

            // // vectors that define the viewport width and height
            delta_u = viewport_u / image_width;
            delta_v = viewport_v / image_height; // Debug #1 had a typo

            // // upper left starting pixel
            auto viewport_upper_left =
                camera_center - (focal_length*w) - viewport_u/2 - viewport_v/2;
            P00 = viewport_upper_left + 0.5 * (delta_u + delta_v);
        }

        color ray_color(const ray& r, const hittable& world, int max_depth) const {
            if (max_depth <= 0) {
                return color(0,0,0);
            }

            hit_record rec;
            if (world.hit(r, interval(0.001, infinity), rec)) {
                color attenuation;
                ray scattered;

                if (rec.material_pointer->scatter(r, rec, attenuation, scattered)){
                    return attenuation*ray_color(scattered, world, max_depth-1);
                }
                return color(0,0,0);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0); // get a value a of a between 0 to 1 based to y coord of ray
            auto white = color(1.0, 1.0, 1.0);
            auto blue = color(0.5, 0.7, 1.0);
            return (1.0-a)*white + a*blue;
        }

        ray get_ray(int i, int j) {
            // Get a randomly sampled camera ray for the pixel at location i,j.
            auto pixel_center = P00 + (i * delta_u) + (j * delta_v);
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