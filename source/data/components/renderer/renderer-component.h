#ifndef RACER_COMPONENT_RENDERER_H_
#define RACER_COMPONENT_RENDERER_H_

#include <component.h>
#include <ray-tracing-renderer/utils/intersection-data.h>
#include <real-time-renderer/utils/mesh.h>
#include <physics/ray.hpp>
#include "material.h"

#define SMALL_NUMBER 1.e-4f

namespace racer
{
    class RendererComponent : public Component
    {
    public:
        virtual void Rastarize() = 0;
        virtual IntersectionData RayTrace(Ray ray) = 0;

        Material rendering_material_;

    private:
        Mesh *renderer_mesh_;
    };
}

#endif // RACER_COMPONENT_RENDERER_H_