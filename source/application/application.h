#ifndef RACER_APPLICATION_APPLICATION_H_
#define RACER_APPLICATION_APPLICATION_H_

#include <iostream>
#include <fstream>
#include <string>

#include <scene.h>
#include <real-time-renderer/real-time-renderer.h>
#include <ray-tracing-renderer/ray-tracing-renderer.h>

#include <scene-importer.h>

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
        RayTracingRendererSystem ray_tracing_renderer_system_;

    private:
        Scene *active_scene_;
    };
}

#endif // RACER_APPLICATION_APPLICATION_H_