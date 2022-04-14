#pragma once

#include <iostream>

#include <json.hpp>
#include <glm/glm.hpp>

#include <component.h>

namespace racer
{
    
    class Light : public Component
    {
        public:
        glm::vec3 color;
        //glm::vec3 position;

        Light()
        {
        }

        Light(nlohmann::json lightData)
        {
            //TO-DO duplicate code to helper functions
            auto positionData = lightData["color"];
            color.r = positionData["r"];
            color.g = positionData["g"];
            color.b = positionData["b"];
        }

    };
} // namespace racer
