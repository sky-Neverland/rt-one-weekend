#include "plane.hpp"
#include <iostream>

Hit Plane::trace(ray & inRay) {
    vec4 origin = (this->worldToModel).transform(inRay.origin);
    vec4 dir = (this->worldToModel).transform(inRay.direction);

    ray ray(origin, dir);

    float t0 = -origin.z/dir.z;
    Hit hit;
    if(t0 >= 0) {
        hit.t = t0;
        hit.pos = inRay.origin + (inRay.direction * t0);
        hit.modelSpacePos = origin + (dir * t0);
        hit.normal = this->getNormal(hit.pos, ray);
        hit.material = this->material;
        hit.brightness = 0;
        hit.inRay = inRay;
    }
    return hit;
}
vec4 Plane::getNormal(vec4 & pos, ray & inRay) {
    vec4 v(0,0,1,0);
    if(v.dot(inRay.direction) > 0) {
        v.z = -1;
    }
    vec4 normal = this->normalToWorld.transform(v);
    normal.w = 0;
    normal.normalize();
    return normal;
}

json Plane::serialize() {
    json json_ = {
        {"type", type},
        {"material", material->serialize()},
        {"transform", modelMatrix.serialize()}
    };

    return json_;
}

Geometry* Plane::deserialize(json json_) {
    MaterialType type = json_["material"]["type"];
    /*
    switch(type) {
        case MaterialType::diffuse: {
            base* m = new base();
            material = m->deserialize(json_["material"]);
        }
        case MaterialType::groundGrid: {
            groundGrid* m = new groundGrid();
            material = m->deserialize(json_["material"]);
        }
        case MaterialType::dielectric: {
            Glass* m = new Glass();
            material = m->deserialize(json_["material"]);
        }
        case MaterialType::conductor: {
            Mirror* m = new Mirror();
            material = m->deserialize(json_["material"]);
        }
        default:
            break;
    }
    material = material->deserialize(json_["material"]);*/
    modelMatrix = modelMatrix.deserialize(json_["transform"]);

    return this;
}