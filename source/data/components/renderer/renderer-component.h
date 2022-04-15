#ifndef RACER_COMPONENT_RENDERER_H_
#define RACER_COMPONENT_RENDERER_H_

#include <glm/glm.hpp>
#include <component.h>
#include <ray-tracing-renderer/utils/intersection-data.h>
#include <real-time-renderer/utils/primitive-model.h>
#include <real-time-renderer/utils/shader/shader.h>
#include <physics/ray.hpp>
#include "material.h"

#define SMALL_NUMBER 1.e-4f

namespace racer
{
    class RendererComponent : public Component
    {
    public:
        virtual void Rastarize(glm::mat4, const Shader &rendering_shader) = 0;
        virtual IntersectionData RayTrace(Ray ray) = 0;

        Material rendering_material_;

    protected:
        PrimitiveModel *renderer_mesh_;
    };
}

#endif // RACER_COMPONENT_RENDERER_H_