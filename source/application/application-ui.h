#ifndef RACER_APPLICATION_APPLICATION_UI_H_
#define RACER_APPLICATION_APPLICATION_UI_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl/imgui_impl_glfw.h>
#include <imgui_impl/imgui_impl_opengl3.h>
#include <stb_image_write.h>
#include <scene.h>

namespace racer
{

    struct RenderProperties
    {
        int width = 400, height = 400;
    };

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
        void RenderRenderResultWindow(ImTextureID);
        void RenderRenderOptionsWindow();
        void RenderSceneComponentHirerchy(Scene*);

        bool IsWindowClosed() const;

        bool RayTraceRender(RenderProperties&);
        void FinishedRayTracing();

    protected:
        ImGuiIO *io;
        bool close_window_ = false;

    private:
        int preview_window_width_, preview_window_height_;
        
        bool start_render_ = false;
        RenderProperties render_properties_;
    };
}

#endif // RACER_APPLICATION_APPLICATION_UI_H_