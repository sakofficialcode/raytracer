#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

class hittable_list : public hittable {
    aabb bbox;
    public:
        std::vector<std::shared_ptr<hittable>> objects;

        hittable_list() {};
        hittable_list(std::shared_ptr<hittable> object) { add(object); }

        void clear() {
            objects.clear();
            bbox = aabb();
        }
        void add(std::shared_ptr<hittable> object) {
            objects.push_back(object);
            bbox = surrounding_box(bbox, object->bounding_box());
        }

        bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override {
            hit_record temp_rec;
            bool any_hit = false;
            double closest = tmax;

            for (const auto& object : objects) {
                if (object->hit(r, tmin, closest, temp_rec)) {
                    any_hit = true;
                    closest = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return any_hit;
        }

        aabb bounding_box() const override {
            return bbox;
        }
};

#endif