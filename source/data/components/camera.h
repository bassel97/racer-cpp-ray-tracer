#ifndef RACER_COMPONENT_CAMERA_H_
#define RACER_COMPONENT_CAMERA_H_

#include <component.h>

#define Rad_Deg_2 0.00872664625

namespace racer
{

    class Camera : public Component
    {
    public:
        Camera(float h_fov = 60.0f, float near_plan = 1.0f, float far_plan = 0.0f)
            : h_fov_(h_fov), near_plane_(near_plan), far_plane_(far_plan)
        {
        }

        float GetVFov(float aspectRatio)
        {
            return 2 * (float)atan(1 / (aspectRatio * tan(h_fov_ * Rad_Deg_2)));
        }

        virtual std::string GetName()
        {
            return "camera";
        }

        float near_plane_;
        float far_plane_;

        float h_fov_;

    protected:
    };

} // namespace racer

#endif // RACER_COMPONENT_CAMERA_H_