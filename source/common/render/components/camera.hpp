#pragma once

#include <json.hpp>

#include <ecs/component.hpp>

#define Rad_Deg_2 0.00872664625

namespace racer
{

    class Camera : public Component
    {
    private:
    public:
        float nearPlane;
        float farPlane;

        float h_fov;

        Camera()
        {
        }

        Camera(nlohmann::json cameraData)
        {
            nearPlane = cameraData["near-plane"];
            farPlane = cameraData["far-plane"];

            h_fov = cameraData["h_fov"];
        }

        float GetVFov(float aspectRatio)
        {
            return 2.0 * atan ( 1 / (aspectRatio * tan ( h_fov * Rad_Deg_2 )) );
        }
    };

} // namespace racer
