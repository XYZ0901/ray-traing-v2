//
// Created by 周诣鑫 on 2023/1/9.
//

#ifndef G101_CAMERA_H
#define G101_CAMERA_H

#include "rtweekend.h"
#include "vec.h"
#include "ray.h"

class camera {
private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
    double time0, time1;
public:
    camera(point3 lookfrom = {0, 0, 0},
           point3 lookat = {0, 0, -1},
           vec3 vup = {0, 1, 0},
           double vfov = 90, // vertical field-of-view in degrees
           double aspect_ratio = 16. / 9.,
           double aperture = 0.,
           double focus_dist = 1.,
           double _time0 = 0., double _time1 = 0.) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2. * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 -
                            vertical / 2 - focus_dist * w;
        lens_radius = aperture / 2;

        time0 = _time0;
        time1 = _time1;
    }

    ray get_ray(double s, double t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return ray{origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset,
                   random_double(time0,time1)};
    }
};

#endif //G101_CAMERA_H
