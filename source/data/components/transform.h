#ifndef RACER_COMPONENT_TRANSFORM_H_
#define RACER_COMPONENT_TRANSFORM_H_

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <component.h>

namespace racer
{
    class Transform : public Component
    {
    public:
        Transform(glm::vec3 = glm::vec3(0.0), glm::vec3 = glm::vec3(0.0), glm::vec3 = glm::vec3(0.0));

        void ResetTransform();
        glm::mat4 GetRotationMatrix();

        virtual std::string GetName()
        {
            return "transform";
        }

        glm::vec3 position_;
        glm::vec3 rotation_;
        glm::vec3 scale_;
    };

} // namespace racer

#endif // RACER_COMPONENT_TRANSFORM_H_