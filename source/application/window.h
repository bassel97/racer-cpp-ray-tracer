#ifndef RACER_APPLICATION_WINDOW_H_
#define RACER_APPLICATION_WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "application-ui.h"

namespace racer
{
    class Window
    {
    public:
        Window();
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
        ~Window();

        void RenderFrame(Scene *, unsigned int, unsigned int);
        void CloseWindow();

        void GetPreviewWindowSize(int &, int &);
        bool RayTraceRender(RenderProperties &);
        void FinishedRayTracing();

        bool IsWindowClosed() const;

    protected:
        GLFWwindow *window_ = nullptr;
        ApplicationUI *application_ui_;

    private:
        const int kStartupWidth = 1024;
        const int kStartupHeight = 720;

        void ConfigureOpenGL();
        void SetWindowIcon();

        bool close_window_ = false;
    };
}

#endif // RACER_APPLICATION_WINDOW_H_