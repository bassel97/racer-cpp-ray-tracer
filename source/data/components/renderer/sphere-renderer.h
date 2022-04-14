#ifndef RACER_COMPONENT_SPHERERENDERER_H_
#define RACER_COMPONENT_SPHERERENDERER_H_

#include <json.hpp>
#include "renderer-component.h"

namespace racer
{
    class SphereRendererComponent : public RendererComponent
    {
    public:
        SphereRendererComponent() {}

        SphereRendererComponent(nlohmann::json shapeData)
        {
            // TO-DO duplicate code to helper functions
            auto materialData = shapeData["material"];
            rendering_material_.color.r = materialData["color"]["r"];
            rendering_material_.color.g = materialData["color"]["g"];
            rendering_material_.color.b = materialData["color"]["b"];

            rendering_material_.Ka = materialData["k"]["ka"];
            rendering_material_.Kd = materialData["k"]["kd"];
            rendering_material_.Kr = materialData["k"]["kr"];
            rendering_material_.Ks = materialData["k"]["ks"];

            rendering_material_.n = materialData["n"];
        }

        IntersectionData RayTrace(Ray ray);

        void Rastarize()
        {
        }
    };
}

#endif // RACER_COMPONENT_SPHERERENDERER_H_