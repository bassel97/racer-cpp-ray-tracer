#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include <json.hpp>

#include <preview-system/online-render-system.hpp>
#include <render/systems/offline-render-system.hpp>

#include <ecs/scene.hpp>

namespace racer {

    class Application{
        std::string fileName;
        std::string outputFileName = "Result.ppm"; 

        void save_image(int Width, int Height, char* fname, unsigned char* pixels) {
            FILE *fp;
            const int maxVal=255; 
            
            printf("Saving image %s: %d x %d\n", fname,Width,Height);
            fp = fopen(fname,"wb");
            if (!fp) {
                    printf("Unable to open file '%s'\n",fname);
                    return;
            }
            fprintf(fp, "P6\n");
            fprintf(fp, "%d %d\n", Width, Height);
            fprintf(fp, "%d\n", maxVal);

            for(int j = 0; j < Height; j++) {
                    fwrite(&pixels[j*Width*3], 3,Width,fp);
            }

            fclose(fp);
        }

        public:

        Application(){}

        int Run(){
            std::ifstream istream("D:/Personal_Projects/racer-cpp-ray-tracer/build/Debug/file.json");
            nlohmann::json j;
            istream >> j;

            std::vector<Scene*> scenes = Scene::PopulateScenes(j["scenes"]);
            
            unsigned char *pixels = new unsigned char [3 * scenes[0]->screen.width * scenes[0]->screen.height];            

            //OnlineRenderSystem onlineRenderSystem;

            OfflineRenderSystem offlineRenderSystem;
            offlineRenderSystem.RenderScene (*scenes[0], pixels);

            save_image(scenes[0]->screen.width, scenes[0]->screen.height, (char*)outputFileName.c_str(), pixels);

            return 0;
        }
    };
}