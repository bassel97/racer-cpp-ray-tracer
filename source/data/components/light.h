#ifndef RACER_COMPONENT_LIGHT_H_
#define RACER_COMPONENT_LIGHT_H_

#include <iostream>
#include <glm/glm.hpp>
#include <component.h>

namespace racer
{
    class Light : public Component
    {
    public:
        Light(glm::vec3 color = glm::vec3(1.0f))
        {
            color_ = color;
        }

        virtual std::string GetName()
        {
            return "light";
        }

        glm::vec3 color_;
    };
} // namespace racer

#endif  // RACER_COMPONENT_LIGHT_H_