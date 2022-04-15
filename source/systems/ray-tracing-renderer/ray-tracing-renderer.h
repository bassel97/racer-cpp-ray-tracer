#ifndef RACER_SYSTEMS_RAYTRACINGRENDERING_H_
#define RACER_SYSTEMS_RAYTRACINGRENDERING_H_

#define M_PI 3.14159265358979323846
#include <glm/glm.hpp>
#include <scene.h>

#include "utils/intersection-data.h"
#include <components/renderer/renderer-component.h>

namespace racer
{
    class RayTracingRendererSystem
    {
    public:
        void RenderScene(Scene *sceneToRender, unsigned char *pixels, int width, int height);

        unsigned int render_result_id_;

    private:
        Scene *scene;
        glm::vec3 GetPixelColor(glm::vec3 origin, glm::vec3 direction, int recursionLevel);
    };
}

#endif // RACER_SYSTEMS_RAYTRACINGRENDERING_H_