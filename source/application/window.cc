#include "window.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

racer::Window::Window()
{
    if (!glfwInit())
        throw "Failed to Initialize GLFW";

    ConfigureOpenGL();

    window_ = glfwCreateWindow(kStartupWidth, kStartupHeight, "RACER", nullptr, nullptr);
    if (!window_)
        throw "Failed to Create Window";

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw "Failed to initialize GLAD";

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    SetWindowIcon();

    application_ui_ = new ApplicationUI(window_);
}

racer::Window::~Window()
{
    delete application_ui_;

    glfwDestroyWindow(window_);
    glfwTerminate();
}

void racer::Window::ConfigureOpenGL()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 0);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
}

void racer::Window::SetWindowIcon()
{
    GLFWimage images[1];
    images[0].pixels = stbi_load("racer-icon.png", &images[0].width, &images[0].height, 0, 4); // rgba channels
    glfwSetWindowIcon(window_, 1, images);
    stbi_image_free(images[0].pixels);
}

void racer::Window::RenderFrame(Scene *scene, unsigned int preview_frame_id, unsigned int rendered_frame_id)
{
    glfwPollEvents();

    application_ui_->StartIMGUIFrame();

    application_ui_->SetIMGUILayout();

    application_ui_->RenderPreviewWindow((ImTextureID)(intptr_t)preview_frame_id);
    application_ui_->RenderRenderResultWindow((ImTextureID)(intptr_t)rendered_frame_id);
    application_ui_->RenderRenderOptionsWindow();

    application_ui_->RenderSceneComponentHirerchy(scene);

    application_ui_->RenderIMGUIFrame();

    // Swap the frame buffers
    glfwSwapBuffers(window_);

    glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (application_ui_->IsWindowClosed() || glfwWindowShouldClose(window_))
        CloseWindow();
}

void racer::Window::CloseWindow()
{
    // // TODO(Bassel): Add save scene on close option.
    close_window_ = true;
}

void racer::Window::GetPreviewWindowSize(int &width, int &height)
{
    application_ui_->GetPreviewWindowSize(width, height);
}

bool racer::Window::RayTraceRender(RenderProperties &render_properties)
{
    return application_ui_->RayTraceRender(render_properties);
}

void racer::Window::FinishedRayTracing()
{
    application_ui_->FinishedRayTracing();
}

bool racer::Window::IsWindowClosed() const
{
    return close_window_;
}