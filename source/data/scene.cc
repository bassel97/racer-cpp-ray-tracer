#include "scene.h"

racer::Scene::Scene(std::string name)
    : name_(name), environment_color_(glm::vec3(0.0f)), active_camera_(NULL)
{
}

racer::Scene::~Scene()
{
    for (auto entity : entities_)
    {
        delete entity;
    }
    entities_.clear();
}

void racer::Scene::AddEntity(Entity *entity)
{
    entities_.insert(entity);
}

void racer::Scene::RemoveEntity(Entity *entity)
{
    entities_.erase(entity);
}