#include <iostream>
#include "vec3.h"
#include "color.h"
#include "ray.h"

bool hit_sphere(const point3& center, double radius, const ray& ray) {
    // dot porduct order does not matter
    vec3 oc = ray.origin() - center;
    auto a = dot(ray.direction(), ray.direction());
    auto b = 2.0 * dot(oc, ray.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = (b*b) - (4*a*c);
    // discriminant describes the square root portion of the quadratic eqn
    // if dis = 0 -> one real root, if dis = 2 -> two roots (ray passes through circle twice)
    // if dis < 0, ray does not hit the circle
    return (discriminant >= 0);
}

color ray_color(const ray& r) {

    if (hit_sphere(point3(0,0,-1), 0.5, r)){

        return color(1,0,0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0); // get a value a of a between 0 to 1 based to y coord of ray
    auto white = color(1.0, 1.0, 1.0);
    auto blue = color(0.5, 0.7, 1.0);
    return (1.0-a)*white + a*blue;
}

int main() {
    // Image
    auto aspect_ratio = 16.0/9.0;
    int image_width = 400;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // // Viewport & Camera
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
    // // auto viewport_width = viewport_height*(static_cast<double>(image_width/image_height));
    auto focal_length = 1.0;
    auto camera_center = point3(0,0,0);

    // // vecotrs that define the viewport width and height
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);
    auto delta_u = viewport_u / image_width;
    auto delta_v = viewport_v / image_height; // Debug #1 had a typo

    // // upper left starting pixel
    auto Q = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;

    auto P00 = vec3(Q.x()+(static_cast<double>(delta_u.x()/2.0)),
    static_cast<double>(Q.y()+(delta_v.y()/2.0)),
        static_cast<double>(Q.z()));
    // auto P00 = Q + 0.5 * (delta_u + delta_v);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // for (int i = 0 ; i < image_width; i++) {
    for (int j = 0; j < image_height; ++j) {
        // std::clog << "scanlines remaining " << (image_height - j) << ' ' << std::flush;

        // for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; ++i) {
            auto pixel_center = vec3(static_cast<double>(P00.x()+(i*delta_u.x())),
                static_cast<double>(P00.y()+(j*delta_v.y())), 
                static_cast<double>(P00.z())
            );
            // auto pixel_center = P00 + (i * delta_u) + (j * delta_v);
            auto ray_direction = pixel_center - camera_center; // direction of ray is from camera to pixel
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            // auto pixel_color = color(double(j) / image_width, double(i) /image_height, 0);
            write_color(std::cout, pixel_color);
        }
    } 
    std::clog << "Done.\n";
}