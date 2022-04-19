#include "scene-importer.h"

racer::Scene *racer::SceneImporter::ImportSceneWithFilePath(std::string file_path)
{
    std::ifstream startupFile(file_path);
    if (!startupFile.fail())
    {
        nlohmann::json json_data;
        startupFile >> json_data;

        return ImportScene(json_data);
    }
    else
    {
        throw "Scene file not found";
    }
}

racer::Scene *racer::SceneImporter::ImportScene(nlohmann::json scene_data)
{
    Scene *new_scene = new Scene(scene_data["name"].get<std::string>());

    for (auto &entity_data : scene_data["data"]["entities"].items())
    {
        Entity *entity = new Entity(entity_data.key());

        for (auto &component_data : entity_data.value()["components"].items())
        {
            if (component_data.key()._Equal("transform"))
            {
                Transform *transform = new Transform(
                    glm::vec3(component_data.value()["position"]["x"], component_data.value()["position"]["y"], component_data.value()["position"]["z"]),
                    glm::vec3(component_data.value()["rotation"]["x"], component_data.value()["rotation"]["y"], component_data.value()["rotation"]["z"]),
                    glm::vec3(component_data.value()["scale"]["x"], component_data.value()["scale"]["y"], component_data.value()["scale"]["z"]));
                entity->AddComponent(transform);
                entity->transform = transform;
                continue;
            }

            if (component_data.key()._Equal("camera"))
            {
                Camera *camera = new Camera(
                    component_data.value()["fov-y"], component_data.value()["near-plane"], component_data.value()["far-plane"]);
                entity->AddComponent(camera);
                new_scene->active_camera_ = camera;
                continue;
            }

            if (component_data.key()._Equal("sphere-renderer"))
            {
                SphereRendererComponent *sphereRendererComponent = new SphereRendererComponent();

                sphereRendererComponent->rendering_material_.color = glm::vec3(
                    component_data.value()["material"]["color"]["r"],
                    component_data.value()["material"]["color"]["g"],
                    component_data.value()["material"]["color"]["b"]);

                sphereRendererComponent->rendering_material_.Ka = component_data.value()["material"]["k"]["ka"];
                sphereRendererComponent->rendering_material_.Kd = component_data.value()["material"]["k"]["kd"];
                sphereRendererComponent->rendering_material_.Kr = component_data.value()["material"]["k"]["kr"];
                sphereRendererComponent->rendering_material_.Ks = component_data.value()["material"]["k"]["ks"];

                sphereRendererComponent->rendering_material_.n = component_data.value()["material"]["n"];
                entity->AddComponent(sphereRendererComponent);
                new_scene->shapes_to_render_.push_back(sphereRendererComponent);
                continue;
            }

            if (component_data.key()._Equal("triangle-renderer"))
            {
                TriangleRendererComponent *triangleRendererComponent = new TriangleRendererComponent();

                triangleRendererComponent->rendering_material_.color = glm::vec3(
                    component_data.value()["material"]["color"]["r"],
                    component_data.value()["material"]["color"]["g"],
                    component_data.value()["material"]["color"]["b"]);

                triangleRendererComponent->rendering_material_.Ka = component_data.value()["material"]["k"]["ka"];
                triangleRendererComponent->rendering_material_.Kd = component_data.value()["material"]["k"]["kd"];
                triangleRendererComponent->rendering_material_.Kr = component_data.value()["material"]["k"]["kr"];
                triangleRendererComponent->rendering_material_.Ks = component_data.value()["material"]["k"]["ks"];

                triangleRendererComponent->rendering_material_.n = component_data.value()["material"]["n"];
                entity->AddComponent(triangleRendererComponent);
                new_scene->shapes_to_render_.push_back(triangleRendererComponent);
                continue;
            }

            if (component_data.key()._Equal("light"))
            {
                Light *light = new Light(
                    glm::vec3(
                        component_data.value()["color"]["r"],
                        component_data.value()["color"]["g"],
                        component_data.value()["color"]["b"]));

                entity->AddComponent(light);
                new_scene->ligths_.push_back(light);
                continue;
            }
        }
        new_scene->AddEntity(entity);
    }
    return new_scene;
}