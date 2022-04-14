#pragma once
#include <glm/glm.hpp>

namespace racer {
    struct IntersectionData {
            bool intersected;
            glm::vec3 pointOfIntersection;
            glm::vec3 normalToIntersection;
            double t;

            IntersectionData():
                intersected(false),
                t(0)
            {}

            IntersectionData(bool intersected, glm::vec3 pointOfIntersection, glm::vec3 normalToIntersection, double t):
                intersected(intersected),
                pointOfIntersection(pointOfIntersection),
                normalToIntersection(normalToIntersection),
                t(t)
            {}
    };
}