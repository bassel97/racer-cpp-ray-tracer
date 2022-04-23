#ifndef RACER_PRIMITIVESHAPE_TRIANGLESHAPE_H_
#define RACER_PRIMITIVESHAPE_TRIANGLESHAPE_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ray-tracing-renderer/utils/intersection-data.h>

namespace racer
{
    class TrianglePrimitiveShape
    {
    public:
        TrianglePrimitiveShape(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
            : v1_(v1), v2_(v2), v3_(v3)
        {
            normal_ = glm::normalize(glm::cross(v1_ - v2_, v2_ - v3_));
        }

        IntersectionData RayTrace(Ray ray, glm::mat4 transform_matrix)
        {
            glm::vec3 normal = glm::vec3(transform_matrix * glm::vec4(normal_, 0.0f));
            glm::vec3 v1 = glm::vec3(transform_matrix * glm::vec4(v1_, 1.0f));
            glm::vec3 v2 = glm::vec3(transform_matrix * glm::vec4(v2_, 1.0f));
            glm::vec3 v3 = glm::vec3(transform_matrix * glm::vec4(v3_, 1.0f));

            float dominator = glm::dot(normal, ray.direction);

            if (dominator >= 0)
                return IntersectionData();

            float t = -(glm::dot(normal, ray.origin) - glm::dot(normal, v1)) / dominator;

            if (t < 0)
                return IntersectionData();

            glm::vec3 p_hit = ray.origin + t * ray.direction;

            glm::vec3 edge0 = v2 - v1;
            glm::vec3 edge1 = v3 - v2;
            glm::vec3 edge2 = v1 - v3;
            glm::vec3 C0 = p_hit - v1;
            glm::vec3 C1 = p_hit - v2;
            glm::vec3 C2 = p_hit - v3;

            if (glm::dot(normal, glm::cross(edge0, C0)) > 0 &&
                glm::dot(normal, glm::cross(edge1, C1)) > 0 &&
                glm::dot(normal, glm::cross(edge2, C2)) > 0)
            {
                return IntersectionData(true, p_hit, normal, t);
            }
            else
            {
                return IntersectionData();
            }
        }

        glm::vec3 v1_;
        glm::vec3 v2_;
        glm::vec3 v3_;

        glm::vec3 normal_;
    };
}
#endif //   RACER_PRIMITIVESHAPE_TRIANGLESHAPE_H_