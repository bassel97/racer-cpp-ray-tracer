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

    Scene *active_scene;

    std::ifstream startupFile("start-up.json");
    if (!startupFile.fail())
    {
        nlohmann::json jsonData;
        startupFile >> jsonData;

        active_scene = new Scene(jsonData["name"], jsonData["data"]);
    }
    else
    {
        active_scene = (new Scene("Empty Scene"));
    }

    // unsigned char *pixels = new unsigned char[3 * scenes[i]->screen.width * scenes[i]->screen.height];

    // previewSystem::OnlineRenderSystem onlineRenderSystem(scenes[i]);

    /*OfflineRenderSystem offlineRenderSystem;
    offlineRenderSystem.RenderScene(scenes[i], pixels);

    stbi_write_png((scenes[i]->name + ".png").c_str(), scenes[i]->screen.width, scenes[i]->screen.height, 3, pixels, scenes[i]->screen.width * 3);
    */

    // return 0;

    RenderProperties render_properties;

    unsigned int texture = 0;
    // texture 1
    // ---------
    glGenTextures(1, &texture);
    /*glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);*/

    while (!window_.IsWindowClosed())
    {
        int width, height;
        window_.GetPreviewWindowSize(width, height);
        real_time_renderer_system_.ResizeTexture(width, height);

        real_time_renderer_system_.RenderScene(active_scene);

        if (window_.RayTraceRender(render_properties))
        {
            unsigned char *pixels = new unsigned char[3 * render_properties.width * render_properties.height];
            ray_tracing_renderer_system_.RenderScene(active_scene, pixels, render_properties.width, render_properties.height);
            stbi_write_png((active_scene->name + ".png").c_str(), render_properties.width, render_properties.height, 3, pixels, render_properties.width * 3);

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_properties.width, render_properties.height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
            glGenerateMipmap(GL_TEXTURE_2D);

            window_.FinishedRayTracing();
        }

        window_.RenderFrame(real_time_renderer_system_.frame_buffer_, texture);
    }

    return 0;
}