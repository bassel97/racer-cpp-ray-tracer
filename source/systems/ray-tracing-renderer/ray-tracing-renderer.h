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
        glm::vec3 GetPixelColor(glm::vec3 origin, glm::vec3 direction, int recursionLevel);

        Scene *scene;
        bool is_rendering_ = false;
        int rendering_index_x = 0, rendering_index_y = 0;
    };
}

#endif // RACER_SYSTEMS_RAYTRACINGRENDERING_H_