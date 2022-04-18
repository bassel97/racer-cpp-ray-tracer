#ifndef RACER_IO_SCENEIMPORTER_H_
#define RACER_IO_SCENEIMPORTER_H_

#include <iostream>
#include <fstream>
#include <scene.h>
#include <json.hpp>

namespace racer
{
    class SceneImporter
    {
    public:
        static Scene *ImportSceneWithFilePath(std::string file_path);
        static Scene *ImportScene(nlohmann::json scene_data);
    };
}

#endif // RACER_IO_SCENEIMPORTER_H_