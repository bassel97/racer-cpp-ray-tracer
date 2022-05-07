#include "ray-tracing-renderer.h"

#include <iostream>
#include <thread>
#include <future>
#include <random>
#include <chrono>
#include <time.h>

#include <cstdlib>
#include <glm/gtc/random.hpp>

inline double random_double()
{
    /*static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);*/

    static thread_local mt19937 generator;
    uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);

    // return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

glm::vec3 random_in_unit_sphere()
{
    while (true)
    {
        auto p = glm::vec3(random_double(), random_double(), random_double());
        if (p.length() >= 1)
            continue;
        return p;
    }
}

glm::vec3 random_unit_vector()
{
    return glm::normalize(random_in_unit_sphere());
}

void racer::RayTracingRendererSystem::RenderScene(Scene *sceneToRender, unsigned char *pixels_array, int width, int height)
{
    scene_ = sceneToRender;
    aspectRatio_ = (float)width / height;
    fov_correction_ = static_cast<float>(tan(scene_->active_camera_->GetTanFOV() / 2.0f));

    width_ = width;
    height_ = height;

    pixels_array_ = pixels_array;

    for (size_t shapeIndex = 0; shapeIndex < scene_->shapes_to_render_.size(); shapeIndex++)
    {
        scene_->shapes_to_render_[shapeIndex]->UpdateBoundingBox();
    }

    rendering_index_x_ = 0;
    rendering_index_y_ = 0;

    is_rendering_ = true;
}

void racer::RayTracingRendererSystem::RenderNextPixel(int rendering_index_x, int rendering_index_y)
{
    glm::mat4 camera_rotation_matrix = scene_->active_camera_->holdingEntity->transform->GetRotationMatrix();

    // TODO (Bassel): Expose those values in GUI
    int samples_per_pixel = 2400;
    float scale = 1.0f / static_cast<float>(samples_per_pixel);

    // TODO (Bassel): Expose those values in GUI
    int number_of_threads = 300;
    // std::thread **pixels_thread = new std::thread *[number_of_threads];
    std::future<glm::vec3> *pixels_thread = new std::future<glm::vec3>[number_of_threads];

    glm::vec3 pixelColor = glm::vec3(0.0);
    for (int s = 0; s < samples_per_pixel; s += number_of_threads)
    {
        for (int thread_index = 0; thread_index < number_of_threads; ++thread_index)
        {
            float u = (2.0f * ((static_cast<float>(rendering_index_x + (random_double() * 2 - 1)) + 0.5f) / width_) - 1.0f) * fov_correction_;
            float v = (1.0f - 2.0f * ((static_cast<float>(rendering_index_y + (random_double() * 2 - 1)) + 0.5f) / height_)) * (fov_correction_ / aspectRatio_);
            float d = -1;

            glm::vec3 direction = {u, v, d};

            glm::vec4 direction_rotated = camera_rotation_matrix * glm::vec4(direction, 1.0f);
            direction = glm::vec3(direction_rotated);

            glm::vec3 cameraOrigin = scene_->active_camera_->holdingEntity->transform->position_;
            // pixelColor += GetPixelColor(cameraOrigin, direction, 0);
            // pixels_thread[thread_index] = new std::thread(&racer::RayTracingRendererSystem::GetPixelColor, this, cameraOrigin, direction, 0);
            pixels_thread[thread_index] = std::async(&racer::RayTracingRendererSystem::GetPixelColor, this, cameraOrigin, direction, 0);
        }
        for (int i = 0; i < number_of_threads; i++)
        {
            // pixels_thread[i]->join();
            pixelColor += pixels_thread[i].get();
        }
    }

    delete[] pixels_thread;

    pixelColor = glm::sqrt(pixelColor * scale);

    pixelColor.r = std::min(1.0f, std::max(0.0f, pixelColor.r));
    pixelColor.g = std::min(1.0f, std::max(0.0f, pixelColor.g));
    pixelColor.b = std::min(1.0f, std::max(0.0f, pixelColor.b));

    pixels_array_[rendering_index_y * width_ * 3 + rendering_index_x * 3] = (char)(pixelColor.r * 255);
    pixels_array_[rendering_index_y * width_ * 3 + rendering_index_x * 3 + 1] = (char)(pixelColor.g * 255);
    pixels_array_[rendering_index_y * width_ * 3 + rendering_index_x * 3 + 2] = (char)(pixelColor.b * 255);
}

void racer::RayTracingRendererSystem::UpdateRender()
{
    if (!is_rendering_)
        return;

    int number_of_threads = 1;
    std::thread **pixels_thread = new std::thread *[number_of_threads];

    for (int i = 0; i < number_of_threads; i++)
    {
        // pixels_thread[i] = new std::thread(&racer::RayTracingRendererSystem::RenderNextPixel, this, rendering_index_x_, rendering_index_y_);
        RenderNextPixel(rendering_index_x_, rendering_index_y_);

        rendering_index_x_++;
        if (rendering_index_x_ >= width_)
        {
            rendering_index_x_ = 0;

            rendering_index_y_++;
            if (rendering_index_y_ >= height_)
            {
                rendering_index_y_ = 0;

                is_rendering_ = false;
            }
        }
    }

    /*for (int i = 0; i < number_of_threads; i++)
    {
        pixels_thread[i]->join();
        delete pixels_thread[i];
    }*/

    delete[] pixels_thread;
}

void racer::RayTracingRendererSystem::UpdateCurrentRenderingProperties(int &current_index_x, int &current_index_y, float &current_percentage)
{
    current_index_x = rendering_index_x_;
    current_index_y = rendering_index_y_;

    current_percentage = (static_cast<float>(rendering_index_y_ * width_ + rendering_index_x_) / static_cast<float>(width_ * height_)) * 100.0f;
}

glm::vec3 racer::RayTracingRendererSystem::GetPixelColor(glm::vec3 origin, glm::vec3 direction, int recursionLevel)
{
    // TODO (Bassel): Expose those values in GUI
    if (recursionLevel >= 25)
        return scene_->environment_color_;

    glm::vec3 pixelColor = {0, 0, 0};

    double tmin = DBL_MAX;
    bool intersected = false;
    IntersectionData nearIntersectedSphereData;
    RendererComponent *nearShape;

    for (size_t shapeIndex = 0; shapeIndex < scene_->shapes_to_render_.size(); shapeIndex++)
    {
        racer::IntersectionData intersectionData = scene_->shapes_to_render_[shapeIndex]->RayTrace(racer::Ray(origin, direction));

        if (intersectionData.intersected)
        {
            if (intersectionData.t < tmin && intersectionData.t > 0)
            {
                intersected = true;
                tmin = intersectionData.t;

                nearIntersectedSphereData = intersectionData;

                nearShape = scene_->shapes_to_render_[shapeIndex];
            }
        }
    }

    if (intersected)
    {
        glm::vec3 view = glm::normalize(origin - nearIntersectedSphereData.pointOfIntersection);
        // glm::vec3 reflectedRay = -view - 2.0f * nearIntersectedSphereData.normalToIntersection * glm::dot(-view, nearIntersectedSphereData.normalToIntersection);

        /*glm::vec3 reflectedRay = -glm::reflect(view, nearIntersectedSphereData.normalToIntersection);

        reflectedRay += glm::normalize(glm::sphericalRand(1.0f));*/

        glm::vec3 reflectedCalculatedRayColor = GetPixelColor(nearIntersectedSphereData.pointOfIntersection,
                                                              glm::normalize(glm::normalize(nearIntersectedSphereData.normalToIntersection) + glm::normalize(glm::sphericalRand(0.5f))), recursionLevel + 1);

        pixelColor = nearShape->rendering_material_.emission + nearShape->rendering_material_.color * reflectedCalculatedRayColor;
    }
    else
    {
        pixelColor = scene_->environment_color_;
    }

    return pixelColor;
}