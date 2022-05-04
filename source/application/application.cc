#include "application.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

racer::Application::Application()
{
}

racer::Application::~Application()
{
}

int racer::Application::Run()
{
    active_scene_ = SceneImporter::ImportSceneWithFilePath("start-up.json");

    RenderProperties render_properties;

    unsigned int texture = 0;

    glGenTextures(1, &texture);

    unsigned char *pixels = new unsigned char[3 * render_properties.width * render_properties.height];
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_properties.width, render_properties.height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    while (!window_.IsWindowClosed())
    {
        int width, height;
        window_.GetPreviewWindowSize(width, height);
        real_time_renderer_system_.ResizeTexture(width, height);

        if (active_scene_->active_camera_ != NULL)
        {
            real_time_renderer_system_.RenderScene(active_scene_);
            ray_tracing_renderer_system_.UpdateRender();
            ray_tracing_renderer_system_.UpdateCurrentRenderingProperties(render_properties.current_x, render_properties.current_y, render_properties.percentage);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_properties.width, render_properties.height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
            glGenerateMipmap(GL_TEXTURE_2D);

            RenderProperties updated_render_properties = render_properties;

            if (window_.RayTraceRender(updated_render_properties))
            {
                render_properties = updated_render_properties;

                delete pixels;
                pixels = new unsigned char[3 * render_properties.width * render_properties.height];
                ray_tracing_renderer_system_.RenderScene(active_scene_, pixels, render_properties.width, render_properties.height);
                //stbi_write_png((active_scene_->name_ + ".png").c_str(), render_properties.width, render_properties.height, 3, pixels, render_properties.width * 3);

                window_.FinishedRayTracing();
            }
        }

        window_.RenderFrame(active_scene_, real_time_renderer_system_.frame_buffer_, texture);
    }

    delete pixels;

    return 0;
}