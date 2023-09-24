#include <iostream>
#include <fstream>
#include "myutility.h"
#include "hittable.h"
#include "hittable_list.h"         
#include "sphere.h"
#include "color.h"
#include "camera.h"
#include <bvh.h>


// color ray_color(const ray& r){
//     return color(0, 0, 0);
// }

// double hit_sphere(const point3& center, double radius, const ray&r) {
//     vec3 oc = r.origin() - center;
//     auto a = r.direction().length_squared();
//     auto half_b = dot(oc, r.direction());
//     auto c = oc.length_squared() - radius * radius;
//     auto discriminant = half_b * half_b - a * c;

//     if(discriminant < 0) {
//         return -1.0;
//     }
//     else {
//         return (-half_b - sqrt(discriminant)) / a;
//     }
// }


// color ray_color(const ray&r) {
//     auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
//     if(t > 0.0) {
//         vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
//         return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
//     }
//     vec3 unit_direction = unit_vector(r.direction());
//     auto a = 0.5*(unit_direction.y() + 1.0);
//     return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
// }

// color ray_color(const ray& r, const hittable& world) {
//     hit_record rec;

//     if(world.hit(r, interval(0, infinity), rec)) {
//         return 0.5 * (rec.normal + color(1, 1, 1));
//     }

//     vec3 unit_direction = unit_vector(r.direction());
//     auto a = 0.5 * (unit_direction.y() + 1.0);
//     return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
// }




int main(){
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -3; a < 3; a++) {
        for (int b = -3; b < 3; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, .5), 0);
                    world.add(make_shared<sphere>(center,  0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    // auto material1 = make_shared<dielectric>(1.5);
    // world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    // auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    // world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    // auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    // world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 50;
    cam.max_depth = 20;

    cam.vfov = 40;
    // cam.lookfrom = point3(10,2,3);
    cam.lookfrom = point3(0 ,2, 8);
    cam.lookat = point3(0,0,0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0.3;
    cam.focus_dist = 10.0;
    cam.focus_dist = 5.0;


    cam.render(world);
}