#ifndef RACER_SYSTEMS_REALTIMERENDERER_H_
#define RACER_SYSTEMS_REALTIMERENDERER_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <scene.h>
#include <preview-system/preview-structs.hpp>
#include <preview-system/primitive-model.hpp>
#include <preview-system/primitive-shapes/cube-preview-data.hpp>
#include <preview-system/primitive-shapes/sphere-preview-data.hpp>
#include <preview-system/shader/shader.hpp>
#include <preview-system/shader/shader-files.hpp>

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

        previewSystem::PrimitiveModel sphere_model_;
        previewSystem::PrimitiveModel cube_model_;

        // TODO (Bassel): Replace with more advanced shader
        previewSystem::Shader rendering_shader_;
    };
}

#endif // RACER_SYSTEMS_REALTIMERENDERER_H_