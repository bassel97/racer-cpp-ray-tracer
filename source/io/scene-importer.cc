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
        return new Scene("New scene");
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
                entity->transform->position_ = glm::vec3(component_data.value()["position"]["x"], component_data.value()["position"]["y"], component_data.value()["position"]["z"]);
                entity->transform->rotation_ = glm::vec3(component_data.value()["rotation"]["x"], component_data.value()["rotation"]["y"], component_data.value()["rotation"]["z"]);
                entity->transform->scale_ = glm::vec3(component_data.value()["scale"]["x"], component_data.value()["scale"]["y"], component_data.value()["scale"]["z"]);
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

                sphereRendererComponent->rendering_material_.emission_color = glm::vec3(
                    component_data.value()["material"]["emission-color"]["r"],
                    component_data.value()["material"]["emission-color"]["g"],
                    component_data.value()["material"]["emission-color"]["b"]);

                sphereRendererComponent->rendering_material_.ambient = component_data.value()["material"]["ambient"];
                sphereRendererComponent->rendering_material_.diffuse = component_data.value()["material"]["diffuse"];
                sphereRendererComponent->rendering_material_.reflectance = component_data.value()["material"]["reflectance"];
                sphereRendererComponent->rendering_material_.specular = component_data.value()["material"]["specular"];
                sphereRendererComponent->rendering_material_.emission = component_data.value()["material"]["emission"];

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

                triangleRendererComponent->rendering_material_.emission_color = glm::vec3(
                    component_data.value()["material"]["emission-color"]["r"],
                    component_data.value()["material"]["emission-color"]["g"],
                    component_data.value()["material"]["emission-color"]["b"]);

                triangleRendererComponent->rendering_material_.ambient = component_data.value()["material"]["ambient"];
                triangleRendererComponent->rendering_material_.diffuse = component_data.value()["material"]["diffuse"];
                triangleRendererComponent->rendering_material_.reflectance = component_data.value()["material"]["reflectance"];
                triangleRendererComponent->rendering_material_.specular = component_data.value()["material"]["specular"];
                triangleRendererComponent->rendering_material_.emission = component_data.value()["material"]["emission"];

                entity->AddComponent(triangleRendererComponent);
                new_scene->shapes_to_render_.push_back(triangleRendererComponent);
                continue;
            }

            if (component_data.key()._Equal("model-renderer"))
            {
                ModelRendererComponent *meshRendererComponent = new ModelRendererComponent(component_data.value()["path"]);

                meshRendererComponent->rendering_material_.color = glm::vec3(
                    component_data.value()["material"]["color"]["r"],
                    component_data.value()["material"]["color"]["g"],
                    component_data.value()["material"]["color"]["b"]);

                meshRendererComponent->rendering_material_.emission_color = glm::vec3(
                    component_data.value()["material"]["emission-color"]["r"],
                    component_data.value()["material"]["emission-color"]["g"],
                    component_data.value()["material"]["emission-color"]["b"]);

                meshRendererComponent->rendering_material_.ambient = component_data.value()["material"]["ambient"];
                meshRendererComponent->rendering_material_.diffuse = component_data.value()["material"]["diffuse"];
                meshRendererComponent->rendering_material_.reflectance = component_data.value()["material"]["reflectance"];
                meshRendererComponent->rendering_material_.specular = component_data.value()["material"]["specular"];
                meshRendererComponent->rendering_material_.emission = component_data.value()["material"]["emission"];

                entity->AddComponent(meshRendererComponent);
                new_scene->shapes_to_render_.push_back(meshRendererComponent);
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