#pragma once

#include <json.hpp>

#include <ecs/component.hpp>

namespace racer
{

    class Camera : public Component
    {
    private:
    public:
        float nearPlane;
        float farPlane;
        float left;
        float right;
        float bottom;
        float top;

        Camera()
        {
        }

        Camera(nlohmann::json cameraData)
        {
            nearPlane = cameraData["near-plane"];
            farPlane = cameraData["far-plane"];
            left = cameraData["left"];
            right = cameraData["right"];
            bottom = cameraData["bottom"];
            top = cameraData["top"];
        }
    };

} // namespace racer
