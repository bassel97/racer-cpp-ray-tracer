#pragma once
#include <physics/ray.hpp>
#include <utilities/graphics/intersection-data.hpp>
#include <shapes/shape.hpp>
#include <algorithm>

#include <ecs/entity.hpp>

namespace racer {

    // A sphere with raduis = 1
    class Sphere : public Shape {
        
        public:

            IntersectionData DidIntersectWithRay(Ray ray) {

                ray.origin = (ray.origin - holdingEntity->transform->position) / holdingEntity->transform->scale;

                glm::vec3 scaledDirection = ray.direction / holdingEntity->transform->scale;

                float A = glm::dot(scaledDirection, scaledDirection);

                float B = - 2 * glm::dot(ray.origin, scaledDirection);

                float C = glm::dot(ray.origin, ray.origin) - 1;

                float D = B * B - 4 * A * C;

                if(D < 0){
                    return IntersectionData();
                }

                float tpos = ( -B + sqrt(D) ) / ( 2.0f * A );
                float tneg = ( -B - sqrt(D) ) / ( 2.0f * A );

                float t = -(SMALL_NUMBER + std::max(tpos, tneg));

                glm::vec3 point = ray.origin + t * scaledDirection;

                glm::vec3 normal = point / holdingEntity->transform->scale;

                normal = glm::normalize(normal);

                glm::vec3 pointOfIntersection = ( point * holdingEntity->transform->scale ) + holdingEntity->transform->position;
                
                glm::vec3 normalToIntersection = normal;

                return IntersectionData(true, pointOfIntersection, normalToIntersection, t);
                
            }

    };

}