#ifndef RACER_COMPONENT_CAMERA_H_
#define RACER_COMPONENT_CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <component.h>

#define Rad_Deg_2 0.00872664625f
#define Rad_Deg 0.0174533f

namespace racer
{

    class Camera : public Component
    {
    public:
        Camera(float fov_y = 60.0f, float near_plan = 1.0f, float far_plan = 0.0f)
            : fov_y_(fov_y), near_plane_(near_plan), far_plane_(far_plan)
        {
        }

        float GetTanFOV()
        {
            return fov_y_ * Rad_Deg;
        }

        virtual std::string GetName()
        {
            return "camera";
        }
        
        glm::mat4 GetViewMatrix();
        glm::mat4 GetProjectionMatrix(float frame_width, float frame_height);

        float near_plane_;
        float far_plane_;

        float fov_y_;

    protected:
    };

} // namespace racer

#endif // RACER_COMPONENT_CAMERA_H_