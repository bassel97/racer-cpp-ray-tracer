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

namespace racer {

    class Application{
        std::string fileName;
        std::string outputFileName = "Result.png"; 

        public:
        Application(){}

        int Run(){
            std::ifstream istream("D:/Personal_Projects/racer-cpp-ray-tracer/build/Debug/file.json");
            nlohmann::json j;
            istream >> j;

            std::vector<Scene*> scenes = Scene::PopulateScenes(j["scenes"]);
            
            unsigned char *pixels = new unsigned char [3 * scenes[0]->screen.width * scenes[0]->screen.height];            

            OnlineRenderSystem onlineRenderSystem;

            OfflineRenderSystem offlineRenderSystem;
            offlineRenderSystem.RenderScene (*scenes[0], pixels);

            stbi_write_png((char*)outputFileName.c_str(), scenes[0]->screen.width, scenes[0]->screen.height, 3, pixels, scenes[0]->screen.width * 3);

            return 0;
        }
    };
}