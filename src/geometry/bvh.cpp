#include "bvh.hpp"
BVH::BVH() {
    min = vec4(1.0/EPSILON, 1.0/EPSILON, 1.0/EPSILON);
    min.w = 1;
    max = min * -1;
    max.w = 1;
}

BVH::BVH(vec4 & min_, vec4 & max_) {
    min = vec4(min_.x, min_.y, min_.z, 1);
    max = vec4(max_.x, max_.y, max_.z, 1);
}

void BVH::setMaterial(Material & m) {
    for(Geometry* child : children) {
        child->setMaterial(m);
    }
}

//this should never be called
vec4 BVH::getNormal(vec4 & pos, ray & inRay) {
    vec4 normal;
    return normal;
}

//perform desired transformations on a mesh object, then call mesh.constructBVH
Hit BVH::trace(ray & inRay) {
    Hit closest;
    vec4 inv_dir(1.0/inRay.direction.x, 1.0/inRay.direction.y, 1.0/inRay.direction.z);

    float tx_min = (min.x - inRay.origin.x)*inv_dir.x;
    float ty_min = (min.y - inRay.origin.y)*inv_dir.y;
    float tz_min = (min.z - inRay.origin.z)*inv_dir.z;

    float tx_max = (max.x - inRay.origin.x)*inv_dir.x;
    float ty_max = (max.y - inRay.origin.y)*inv_dir.y;
    float tz_max = (max.z - inRay.origin.z)*inv_dir.z;

    float tx_enter = std::min(tx_min, tx_max);
    float tx_exit = std::max(tx_min, tx_max);

    float ty_enter = std::min(ty_min, ty_max);
    float ty_exit = std::max(ty_min, ty_max);

    float tz_enter = std::min(tz_min ,tz_max);
    float tz_exit = std::max(tz_min ,tz_max);

    float t_enter = std::max(std::max(tx_enter, ty_enter), tz_enter);
    float t_exit = std::min(std::min(tx_exit, ty_exit), tz_exit);

    if(t_enter < t_exit && t_exit > 0) {
        for(Geometry* child : children) {
            Hit hit = child->trace(inRay);
            if(hit.t < closest.t) {
                closest = hit;
            }
        }
    }
    return closest;
}

BVH BVH::include(vec4 & point) {
    min.x = point.x < min.x ? point.x : min.x;
    min.y = point.y < min.y ? point.y : min.y;
    min.z = point.z < min.z ? point.z : min.z;

    max.x = point.x > max.x ? point.x : max.x;
    max.y = point.y > max.y ? point.y : max.y;
    max.z = point.z > max.z ? point.z : max.z;

    return *this;
}

BVH BVH::coalesce(BVH & other) {
    return *this;
}

BVH BVH::transform(mat4 & m) {
    BVH output = BVH();
    vec4 corner_0 = vec4(min.x, min.y, min.z, 1);
    vec4 corner_1 = vec4(max.x, min.y, min.z, 1);
    vec4 corner_2 = vec4(min.x, max.y, min.z, 1);
    vec4 corner_3 = vec4(min.x, min.y, max.z, 1);
    vec4 corner_4 = vec4(max.x, max.y, min.z, 1);
    vec4 corner_5 = vec4(max.x, min.y, max.z, 1);
    vec4 corner_6 = vec4(min.x, max.y, max.z, 1);
    vec4 corner_7 = vec4(max.x, max.y, max.z, 1);

    corner_0 = m.transform(corner_0);
    corner_1 = m.transform(corner_1);
    corner_2 = m.transform(corner_2);
    corner_3 = m.transform(corner_3);
    corner_4 = m.transform(corner_4);
    corner_5 = m.transform(corner_5);
    corner_6 = m.transform(corner_6);
    corner_7 = m.transform(corner_7);

    output.include(corner_0);
    output.include(corner_1);
    output.include(corner_2);
    output.include(corner_3);
    output.include(corner_4);
    output.include(corner_5);
    output.include(corner_6);
    output.include(corner_7);

    return output;
}