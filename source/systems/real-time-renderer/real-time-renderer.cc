#include "real-time-renderer.h"

racer::RealtimeRendererSystem::RealtimeRendererSystem(int width, int height) : rendering_shader_(kMVPVertexShader, kDiffuseFragmentShader)
{
    render_frame_width_ = width;
    render_frame_height_ = height;

    // framebuffer configuration
    // -------------------------
    glGenFramebuffers(1, &frame_buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);
    glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    // create a color attachment texture
    glGenTextures(1, &texture_Colorbuffer_);
    glBindTexture(GL_TEXTURE_2D, texture_Colorbuffer_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_Colorbuffer_, 0);

    glGenRenderbuffers(1, &render_buffer_object_);
    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_object_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);                                     // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer_object_); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

racer::RealtimeRendererSystem::~RealtimeRendererSystem()
{
}

void racer::RealtimeRendererSystem::ResizeTexture(int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, texture_Colorbuffer_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_object_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.

    render_frame_width_ = width;
    render_frame_height_ = height;

    glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
}

void racer::RealtimeRendererSystem::RenderScene(Scene *scene)
{
    // bind to framebuffer and draw scene as we normally would to color texture
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_);

    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(scene->environment_color_.r, scene->environment_color_.g, scene->environment_color_.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glm::mat4 projection = scene->active_camera_->GetProjectionMatrix(render_frame_width_, render_frame_height_);

    // camera/view transformation
    scene->active_camera_->holdingEntity->transform->position_.z += 5;
    glm::mat4 view = scene->active_camera_->GetViewMatrix();
    scene->active_camera_->holdingEntity->transform->position_.z -= 5;

    for (int i = 0; i < scene->shapes_to_render_.size(); i++)
    {
        rendering_shader_.use();

        rendering_shader_.setVec3("viewDir", glm::vec3(0, 0, -1));

        scene->shapes_to_render_[i]->Rastarize(projection * view, rendering_shader_);

        if (scene->shapes_to_render_[i]->holdingEntity->IsActive())
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glm::vec3 temp_color = scene->shapes_to_render_[i]->rendering_material_.color;
            scene->shapes_to_render_[i]->rendering_material_.color = glm::vec3(1.0f);
            scene->shapes_to_render_[i]->Rastarize(projection * view, rendering_shader_);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            scene->shapes_to_render_[i]->rendering_material_.color = temp_color;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}