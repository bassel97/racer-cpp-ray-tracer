#ifndef RACER_COMPONENT_SPHERERENDERER_H_
#define RACER_COMPONENT_SPHERERENDERER_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <json.hpp>
#include "renderer-component.h"
#include <real-time-renderer/utils/primitive-model.h>
#include <real-time-renderer/utils/primitive-shapes/sphere-preview-data.h>

namespace racer
{
    class SphereRendererComponent : public RendererComponent
    {
    public:
        SphereRendererComponent();
        SphereRendererComponent(nlohmann::json shapeData);
        ~SphereRendererComponent();

        IntersectionData RayTrace(Ray ray);

        void Rastarize(glm::mat4 VP, const Shader &rendering_shader);

        virtual std::string GetName()
        {
            return "sphere renderer";
        }
    };
}

#endif // RACER_COMPONENT_SPHERERENDERER_H_