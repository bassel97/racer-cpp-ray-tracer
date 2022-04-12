#pragma once

#include <glm/glm.hpp>

#include <json.hpp>

#include <component.hpp>

namespace racer
{

    class Transform : public Component
    {
    private:
    public:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        Transform()
        {
            position.x = 0.0f;
            position.y = 0.0f;
            position.z = 0.0f;

            rotation.x = 0.0f;
            rotation.y = 0.0f;
            rotation.z = 0.0f;

            scale.x = 1.0f;
            scale.y = 1.0f;
            scale.z = 1.0f;
        }

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
    };

} // namespace racer