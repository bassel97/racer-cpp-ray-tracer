#pragma once
#include <glm/glm.hpp>

namespace racer
{    
    class Material 
    {
        public:
        glm::vec3 color = glm::vec3(0.0);
        glm::vec3 emission_color = glm::vec3(0.0);
        float ambient = 0, diffuse = 0, specular = 0, reflectance = 0, emission = 0;
    };
}