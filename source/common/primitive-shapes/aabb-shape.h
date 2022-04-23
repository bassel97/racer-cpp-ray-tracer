#ifndef RACER_PRIMITIVESHAPE_AABBSHAPE_H_
#define RACER_PRIMITIVESHAPE_AABBSHAPE_H_

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ray-tracing-renderer/utils/intersection-data.h>

namespace racer
{
    class AABBPrimitiveShape
    {
    public:
        AABBPrimitiveShape(glm::vec3 min_bound, glm::vec3 max_bound)
            : min_bound_(min_bound), max_bound_(max_bound)
        {
        }

        void UpdateBorders(glm::mat4 transform_matrix)
        {
            min_bound_corrected = glm::vec3(transform_matrix * glm::vec4(min_bound_, 1.0));
            max_bound_corrected = glm::vec3(transform_matrix * glm::vec4(max_bound_, 1.0));
        }

        bool RayHit(Ray ray)
        {
            float tmin = (min_bound_corrected.x - ray.origin.x) / ray.direction.x;
            float tmax = (max_bound_corrected.x - ray.origin.x) / ray.direction.x;

            if (tmin > tmax)
                std::swap(tmin, tmax);

            float tymin = (min_bound_corrected.y - ray.origin.y) / ray.direction.y;
            float tymax = (max_bound_corrected.y - ray.origin.y) / ray.direction.y;

            if (tymin > tymax)
                std::swap(tymin, tymax);

            if ((tmin > tymax) || (tymin > tmax))
                return false;

            if (tymin > tmin)
                tmin = tymin;

            if (tymax < tmax)
                tmax = tymax;

            float tzmin = (min_bound_corrected.z - ray.origin.z) / ray.direction.z;
            float tzmax = (max_bound_corrected.z - ray.origin.z) / ray.direction.z;

            if (tzmin > tzmax)
                std::swap(tzmin, tzmax);

            if ((tmin > tzmax) || (tzmin > tmax))
                return false;

            if (tzmin > tmin)
                tmin = tzmin;

            if (tzmax < tmax)
                tmax = tzmax;

            return true;
        }

        glm::vec3 min_bound_, max_bound_;
        glm::vec3 min_bound_corrected;
        glm::vec3 max_bound_corrected;
    };
}
#endif //   RACER_PRIMITIVESHAPE_AABBSHAPE_H_