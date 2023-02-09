//
// Created by 周诣鑫 on 2023/1/9.
//

#ifndef G101_RAY2_H
#define G101_RAY2_H

#include "vec.h"

class ray {
public:
    ray() = default;

    ray(const point3 &origin, const vec3 &direction, double time = 0.) :
            orig(origin), dir(direction), tm(time) {}

    point3 origin() const { return orig; }

    point3 direction() const { return dir; }

    double time() const { return tm; }

    point3 at(double t) const {
        return orig + t * dir;
    }

public:
    point3 orig;
    vec3 dir;
    double tm;
};

#endif //G101_RAY2_H
