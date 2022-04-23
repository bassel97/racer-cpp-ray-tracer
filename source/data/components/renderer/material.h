#pragma once
#include <glm/glm.hpp>

namespace racer
{    
    class Material 
    {
        public:
        glm::vec3 color = glm::vec3(0.0);
        float Ka = 0, Kd = 0, Ks = 0, Kr = 0;
        int n = 50;
    };
}