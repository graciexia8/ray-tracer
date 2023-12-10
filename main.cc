#include "util.h"

#include <iostream>
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "bvh.h"

int main() {

    // World

    hittable_list world;

    for (int a = -5; a < 5; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
                shared_ptr<material> sphere_material;
                auto albedo = color::random() * color::random();
                sphere_material = make_shared<lambertian>(albedo);
                auto center2 = center + vec3(0, random_double(0,0.5), 0);
                world.add(make_shared<Sphere>(center, center2, 0.2, sphere_material));
                // if (choose_mat < 0.4) {
                //     // diffuse
                //     auto albedo = color::random() * color::random();
                //     sphere_material = make_shared<lambertian>(albedo);
                //     auto center2 = center + vec3(0, random_double(0,1), 0);
                //     world.add(make_shared<Sphere>(center, center2, 0.2, sphere_material));
                // } else if (choose_mat < 0.95) {
                //     // metal
                //     auto albedo = color::random(0.5, 1);
                //     auto fuzz = random_double(0, 0.5);
                //     sphere_material = make_shared<metal>(albedo, fuzz);
                //     world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                // } else {
                //     // glass
                //     sphere_material = make_shared<dielectric>(1.5);
                //     world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                // }

        }
    }
    // auto distance = 0.4;
    // for (int a = 0; a > -3; a--) {
    //     for (int b = 1; b < 4; b++) {
    //         for (int c = 0; c < 3; c++) {
    //             auto choose_mat = random_double();
    //             point3 center(c*distance,b*distance,a*distance);
    //             auto albedo = color::random() * color::random();

    //             if (choose_mat < 0.8) {
    //                 // diffuse
    //                 auto albedo = color::random() * color::random();
    //                 auto sphere_material = make_shared<lambertian>(albedo);
    //                 world.add(make_shared<Sphere>(center, 0.2, sphere_material));
    //             } else if (choose_mat < 0.95) {
    //                 // metal
    //                 auto albedo = color::random(0.5, 1);
    //                 auto fuzz = random_double(0, 0.5);
    //                 auto sphere_material = make_shared<metal>(albedo, fuzz);
    //                 world.add(make_shared<Sphere>(center, 0.2, sphere_material));
    //             } else {
    //                 // glass
    //                 auto sphere_material = make_shared<dielectric>(1.5);
    //                 world.add(make_shared<Sphere>(center, 0.2, sphere_material));
    //             }

    //         }
    //     }
    // }
    
    
    auto material2 = make_shared<lambertian>(color(0.4, 0.8, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.1, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));


    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));

    std::clog << "Objects in world: " << world.length() << '\n';

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 400;
    cam.samples_per_pixel = 10;
    cam.max_depth = 20;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);
    
    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;
    cam.render(world);

}