#include "application.h"

#include <iostream>

racer::Application::Application()
{
}

racer::Application::~Application()
{
}

int racer::Application::Run()
{

    Scene *active_scene;

    std::ifstream startupFile("E:\\Bassel\\Racer\\racer-cpp-ray-tracer\\build\\Debug\\start-up.json");
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

    while (!window_.IsWindowClosed())
    {
        int width, height;
        window_.GetPreviewWindowSize(width, height);
        std::cout << width << " " << height << std::endl;
        real_time_renderer_system_.ResizeTexture(width, height);
        real_time_renderer_system_.RenderScene(active_scene);
        window_.RenderFrame(real_time_renderer_system_.framebuffer);
    }

    return 0;
}