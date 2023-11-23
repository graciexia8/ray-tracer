#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "vec3.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object);}

        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }
        
        // only want to store a record of the closest object hit
        bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {

            auto closestMax = ray_tmax;
            hit_record temp_rec;
            bool hitAnything = false;

            for (const auto& object: objects) {
                if (object->hit(r, ray_tmin, closestMax, temp_rec)) {
                    closestMax = rec.t;
                    rec = temp_rec;
                    hitAnything = true;
                }
            }

            return hitAnything;
        }


};

#endif