#ifndef RACER_APPLICATION_APPLICATION_H_
#define RACER_APPLICATION_APPLICATION_H_

#include <iostream>
#include <fstream>
#include <string>

#include <json.hpp>

#include <render/systems/offline-render-system.hpp>

#include <scene.h>
#include <real-time-renderer/real-time-renderer.h>

#include "window.h"

namespace racer
{
    class Application
    {
    public:
        Application();
        Application(const Application &) = delete;
        Application &operator=(const Application &) = delete;
        ~Application();
        
        int Run();

    protected:
        Window window_;
        RealtimeRendererSystem real_time_renderer_system_;
    };
}

#endif // RACER_APPLICATION_APPLICATION_H_