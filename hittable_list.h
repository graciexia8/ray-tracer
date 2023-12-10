#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "util.h"
#include "hittable.h"
#include "aabb.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    private:
        aabb bbox;
    public:
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { 
            add(object);
            bbox = aabb(bbox, object->bounding_box());
        }

        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
            bbox = aabb(bbox, object->bounding_box());
        }
        int length() {
            return objects.size();
        }

        aabb bounding_box() const override {
            return bbox;
        }
        
        // only want to store a record of the closest object hit
        bool hit(const ray& r, interval ray_t,  hit_record& rec) const override {

            auto closestMax = ray_t.max;
            hit_record temp_rec;
            bool hitAnything = false;

            for (const auto& object: objects) {
                if (object->hit(r, interval(ray_t.min, closestMax), temp_rec)) {
                    if (temp_rec.t < closestMax) {
                        closestMax = temp_rec.t;
                        rec = temp_rec;
                        hitAnything = true; 
                    }
                }
            }

            return hitAnything;
        }


};

#endif