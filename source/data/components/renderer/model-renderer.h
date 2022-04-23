#ifndef RACER_COMPONENT_MODELRENDERER_H_
#define RACER_COMPONENT_MODELRENDERER_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer-component.h"
#include <real-time-renderer/utils/primitive-model.h>
#include <real-time-renderer/utils/primitive-shapes/sphere-preview-data.h>
#include <render/model.h>

#include <primitive-shapes/triangle-shape.h>

namespace racer
{
    class ModelRendererComponent : public RendererComponent
    {
    public:
        ModelRendererComponent(string const &path)
        {
            model = new Model(path, model_triangles_);
        }

        ~ModelRendererComponent()
        {
            delete model;
        }

        IntersectionData RayTrace(Ray ray);

        void Rastarize(glm::mat4 VP, const Shader &rendering_shader);

        virtual std::string GetName()
        {
            return "model renderer";
        }

        Model *model;
        vector<TrianglePrimitiveShape> model_triangles_;
    };
}

#endif // RACER_COMPONENT_MODELRENDERER_H_