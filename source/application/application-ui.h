#ifndef RACER_APPLICATION_APPLICATION_UI_H_
#define RACER_APPLICATION_APPLICATION_UI_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl/imgui_impl_glfw.h>
#include <imgui_impl/imgui_impl_opengl3.h>

namespace racer
{
    class ApplicationUI
    {
    public:
        ApplicationUI(GLFWwindow *glfw_window);
        ApplicationUI(const ApplicationUI &) = delete;
        ApplicationUI &operator=(const ApplicationUI &) = delete;
        ~ApplicationUI();

        void StartIMGUIFrame();
        void SetIMGUILayout();
        void RenderIMGUIFrame();

        void GetPreviewWindowSize(int &, int &);
        void RenderPreviewWindow(ImTextureID);

        bool IsWindowClosed() const;

        ImGuiIO *io;

    protected:
        bool close_window_ = false;

    private:
        int preview_window_width_, preview_window_height_;
    };
}

#endif // RACER_APPLICATION_APPLICATION_UI_H_