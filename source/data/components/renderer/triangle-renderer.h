#ifndef RACER_COMPONENT_TRIANGLERENDERER_H_
#define RACER_COMPONENT_TRIANGLERENDERER_H_

#include "renderer-component.h"
#include <real-time-renderer/utils/primitive-shapes/triangle-preview-data.h>
#include <primitive-shapes/triangle-shape.h>

namespace racer
{
    class TriangleRendererComponent : public RendererComponent
    {
    public:
        TriangleRendererComponent();
        ~TriangleRendererComponent();

        IntersectionData RayTrace(Ray ray);

        void Rastarize(glm::mat4 VP, const Shader &rendering_shader);

        virtual std::string GetName()
        {
            return "triangle renderer";
        }

        TrianglePrimitiveShape trianglePrimitiveShape;
    };
}

#endif // RACER_COMPONENT_TRIANGLERENDERER_H_