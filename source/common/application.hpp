#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include <json.hpp>

#include <preview-system/online-render-system.hpp>
#include <render/systems/offline-render-system.hpp>

#include <ecs/scene.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace racer
{

    class Application
    {

    public:
        Application() {}

        int Run()
        {

            std::vector<Scene *> scenes;

            std::ifstream startupFile("start-up.json");
            if (!startupFile.fail())
            {
                nlohmann::json jsonData;
                startupFile >> jsonData;

                scenes = Scene::PopulateScenes(jsonData["scenes"]);
            }
            else
            {
                scenes.push_back(new Scene("Empty Scene"));
            }

            for (int i = 0; i < scenes.size(); i++)
            {
                unsigned char *pixels = new unsigned char[3 * scenes[i]->screen.width * scenes[i]->screen.height];

                previewSystem::OnlineRenderSystem onlineRenderSystem(scenes[i]);

                OfflineRenderSystem offlineRenderSystem;
                offlineRenderSystem.RenderScene(scenes[i], pixels);

                stbi_write_png((scenes[i]->name + ".png").c_str(), scenes[i]->screen.width, scenes[i]->screen.height, 3, pixels, scenes[i]->screen.width * 3);
            }

            return 0;
        }
    };
}