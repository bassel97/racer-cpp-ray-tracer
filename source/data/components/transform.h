#ifndef RACER_COMPONENT_TRANSFORM_H_
#define RACER_COMPONENT_TRANSFORM_H_

#include <glm/glm.hpp>

#include <json.hpp>

#include <component.h>

namespace racer
{
    class Transform : public Component
    {
    public:
        Transform();

        Transform(nlohmann::json transformData)
        {
            // TO-DO duplicate code to helper functions
            auto positionData = transformData["position"];
            position.x = positionData["x"];
            position.y = positionData["y"];
            position.z = positionData["z"];

            auto rotationData = transformData["rotation"];
            rotation.x = rotationData["x"];
            rotation.y = rotationData["y"];
            rotation.z = rotationData["z"];

            auto scaleData = transformData["scale"];
            scale.x = scaleData["x"];
            scale.y = scaleData["y"];
            scale.z = scaleData["z"];
        }

        void ResetTransform();

        virtual std::string GetName()
        {
            return "transform";
        }

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };

} // namespace racer

#endif // RACER_COMPONENT_TRANSFORM_H_