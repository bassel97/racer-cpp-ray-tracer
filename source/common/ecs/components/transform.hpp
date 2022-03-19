#pragma once

#include <glm/glm.hpp>

#include <json.hpp>

#include <ecs/component.hpp>

namespace racer
{

    class Transform : public Component {
        private:

        public:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        Transform (nlohmann::json transformData)
        {
            //TO-DO duplicate code to helper functions
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