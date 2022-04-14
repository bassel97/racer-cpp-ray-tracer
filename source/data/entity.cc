#include "entity.h"

racer::Entity::Entity(std::string entityName, nlohmann::json entityData) : name(entityName)
{
    auto componentsData = entityData["components"];
    for (auto &componentData : componentsData.items())
    {
        if (componentData.key()._Equal("transform"))
        {
            Transform *transformComponent = new Transform(componentData.value());
            AddComponent(transformComponent);
            transform = transformComponent;
            continue;
        }
        if (componentData.key()._Equal("camera"))
        {
            Camera *cameraComponent = new Camera(componentData.value());
            AddComponent(cameraComponent);
            continue;
        }
        if (componentData.key()._Equal("light"))
        {
            Light *lightComponent = new Light(componentData.value());
            AddComponent(lightComponent);
            continue;
        }
        if (componentData.key()._Equal("sphere-renderer"))
        {
            SphereRendererComponent *sphereComponent = new SphereRendererComponent(componentData.value());
            AddComponent(sphereComponent);
            continue;
        }
    }
}

void racer::Entity::AddComponent(Component *component)
{
    components.insert(component);
    component->holdingEntity = this;
}

void racer::Entity::RemoveComponent(Component *component)
{
    components.erase(component);
}

std::string racer::Entity::GetName() const
{
    return name;
}

std::vector<racer::Entity *> racer::Entity::PopulateEntities(nlohmann::json entities)
{
    std::vector<Entity *> entitiesVector;
    for (auto &entityData : entities.items())
    {
        Entity *entity = new Entity(entityData.key(), entityData.value());
        entitiesVector.push_back(entity);
    }
    return entitiesVector;
}