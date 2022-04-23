#include "entity.h"

racer::Entity::Entity(std::string entity_name) : name_(entity_name), is_active_(false)
{
    Transform *transform_comp = new Transform();
    AddComponent(transform_comp);
    transform = transform_comp;
}

racer::Entity::~Entity()
{
    for (auto component : components_)
    {
        delete component;
    }
    components_.clear();
}

void racer::Entity::AddComponent(Component *component)
{
    components_.insert(component);
    component->holdingEntity = this;
}

void racer::Entity::RemoveComponent(Component *component)
{
    components_.erase(component);
}

std::string racer::Entity::GetName() const
{
    return name_;
}