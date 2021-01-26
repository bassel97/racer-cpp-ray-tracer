#pragma once
#include <glm/glm.hpp>

namespace racer {

    // A function for ray
    class Ray {

        public:

            glm::vec3 origin;
            glm::vec3 direction;

            Ray()
            {}

            Ray(glm::vec3 origin, glm::vec3 direction):
                origin(origin),
                direction(direction)
            {}

    };

}