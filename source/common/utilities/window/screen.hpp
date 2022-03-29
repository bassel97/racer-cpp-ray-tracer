#pragma once

#include <json.hpp>

namespace racer
{
    class Screen
    {

    public:
        int width;
        int height;

        float aspectRatio;

        Screen() {}

        Screen(int width, int height) : width(width), height(height)
        {
        }

        Screen(nlohmann::json screenData)
        {
            SetScreen(screenData);
        }

        void SetScreen(nlohmann::json screenData)
        {
            width = screenData["width"];
            height = screenData["height"];

            aspectRatio = (float)width / height;
        }

        void SetScreen(int width, int height)
        {
            this->width = width;
            this->height = height;
        }
    };
} // namespace racer
