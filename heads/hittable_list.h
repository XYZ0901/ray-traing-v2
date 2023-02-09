//
// Created by 周诣鑫 on 2023/1/8.
//

#ifndef G101_HITTABLE_LIST_H
#define G101_HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects;

public:
    hittable_list() = default;

    explicit hittable_list(const shared_ptr<hittable> &object) {
        add(object);
    }

    void clear() { objects.clear(); }

    void add(const shared_ptr<hittable> &object) {
        objects.push_back(object);
    }

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double time0, double time1, aabb &output_box) const override;
};

bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto &object: objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1, aabb &output_box) const {
    if (objects.empty()) {
        return false;
    }
    aabb temp_box;
    bool first_box = true;

    for (const auto &obj: objects) {
        if (!obj->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }
    return true;
}

#endif //G101_HITTABLE_LIST_H
