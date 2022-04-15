#pragma once

#include <json.hpp>

#include <component.h>

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
            nearPlane = 1.0f;
            farPlane = 0.0f;

            h_fov = 90.0f;
        }

        Camera(nlohmann::json cameraData)
        {
            nearPlane = cameraData["near-plane"];
            farPlane = cameraData["far-plane"];

            h_fov = cameraData["h_fov"];
        }

        float GetVFov(float aspectRatio)
        {
            return 2 * (float)atan ( 1 / (aspectRatio * tan ( h_fov * Rad_Deg_2 )) );
        }

        virtual std::string GetName()
        {
            return "camera";
        }
    };

} // namespace racer
