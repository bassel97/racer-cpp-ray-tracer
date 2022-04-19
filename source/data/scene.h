#ifndef RACER_DATA_SCENE_H_
#define RACER_DATA_SCENE_H_

#include <iostream>
#include <set>
#include <vector>

#include <entity.h>

#include <utilities/window/screen.hpp>

namespace racer
{
    class Scene
    {
    public:
        Scene(std::string = "");
        Scene(const Scene &) = delete;
        Scene &operator=(const Scene &) = delete;
        ~Scene();

        void AddEntity(Entity *entity);
        void RemoveEntity(Entity *entity);
        int GetEntitiesCount() const
        {
            return static_cast<int>(entities_.size());
        }

        std::vector<RendererComponent *> shapes_to_render_;
        std::vector<Light *> ligths_;
        std::set<Entity *> entities_;
        Camera *active_camera_;
        glm::vec3 environment_color_;
        std::string name_;
    };
} // namespace racer

#endif // RACER_DATA_SCENE_H_