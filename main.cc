#include "util.h"

#include <iostream>
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, interval(0, infinity), rec)) {
        return color(rec.normal.x(), rec.normal.y(), rec.normal.z());
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0); // get a value a of a between 0 to 1 based to y coord of ray
    auto white = color(1.0, 1.0, 1.0);
    auto blue = color(0.5, 0.7, 1.0);
    return (1.0-a)*white + a*blue;
}

int main() {

    // World

    hittable_list world;

    world.add(make_shared<Sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<Sphere>(point3(0,-100.5,-1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 400;

    cam.render(world);

}