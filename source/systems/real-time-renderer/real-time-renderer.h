#ifndef RACER_SYSTEMS_REALTIMERENDERER_H_
#define RACER_SYSTEMS_REALTIMERENDERER_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <scene.h>
#include "utils/primitive-model.h"
#include "utils/primitive-shapes/cube-preview-data.h"
#include "utils/primitive-shapes/sphere-preview-data.h"
#include "utils/shader/shader.h"
#include "utils/shader/shader-files.h"

namespace racer
{
    class RealtimeRendererSystem
    {
    public:
        RealtimeRendererSystem(int width = 1024, int height = 720);
        ~RealtimeRendererSystem();
        void ResizeTexture(int, int);
        void RenderScene(Scene *);

        unsigned int frame_buffer_;

    private:
        int render_frame_width_, render_frame_height_;
        unsigned int texture_Colorbuffer_;
        unsigned int render_buffer_object_;

        // TODO (Bassel): Replace with more advanced shader
        Shader rendering_shader_;
    };
}

#endif // RACER_SYSTEMS_REALTIMERENDERER_H_