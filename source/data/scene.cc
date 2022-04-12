#include "scene.h"

racer::Scene::Scene(std::string name) : name(name)
{
    environmentColor.r = 0.666666f;
    environmentColor.g = 0.666666f;
    environmentColor.b = 0.666666f;

    screen.SetScreen(512, 512);

    nlohmann::json emptyCameraData;
    Entity *cameraEntity = new Entity("Camera", emptyCameraData);

    Camera *cameraComponent = new Camera();
    cameraEntity->AddComponent(cameraComponent);

    Transform *transformComponent = new Transform();
    cameraEntity->AddComponent(transformComponent);
    cameraEntity->transform = transformComponent;

    activeCamera = cameraComponent;
}

racer::Scene::Scene(std::string name, nlohmann::json sceneData) : name(name)
{
    std::vector<Entity *> entitiesVector = Entity::PopulateEntities(sceneData["entities"]);
    for (auto &entity : entitiesVector)
    {
        AddEntity(entity);

        Camera *cameraComponent = entity->GetComponent<Camera>();
        if (cameraComponent != NULL)
        {
            cameras.push_back(cameraComponent);
            activeCamera = cameraComponent;
        }

        Shape *shapeComponent = entity->GetComponent<Shape>();
        if (shapeComponent != NULL)
            shapesToRender.push_back(shapeComponent);

        Light *lightComponent = entity->GetComponent<Light>();
        if (lightComponent != NULL)
            ligths.push_back(lightComponent);
    }

    screen.SetScreen(sceneData["scene-properties"]["screen"]);

    environmentColor.r = sceneData["scene-properties"]["environment-color"]["r"];
    environmentColor.g = sceneData["scene-properties"]["environment-color"]["g"];
    environmentColor.b = sceneData["scene-properties"]["environment-color"]["b"];
}

void racer::Scene::AddEntity(Entity *entity)
{
    entities.insert(entity);
}

void racer::Scene::RemoveEntity(Entity *entity)
{
    entities.erase(entity);
}

std::vector<racer::Scene *> racer::Scene::PopulateScenes(nlohmann::json scenes)
{
    std::vector<Scene *> scenesVector;
    for (auto &sceneData : scenes.items())
    {
        Scene *scene = new Scene(sceneData.key(), sceneData.value());
        scenesVector.push_back(scene);
    }
    return scenesVector;
}