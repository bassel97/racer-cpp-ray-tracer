#ifndef RACER_SYSTEMS_REALTIMERENDERER_H_
#define RACER_SYSTEMS_REALTIMERENDERER_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <scene.h>
#include <preview-system/preview-structs.hpp>
#include <preview-system/primitive-model.hpp>
#include <preview-system/primitive-shapes/cube-preview-data.hpp>
#include <preview-system/primitive-shapes/sphere-preview-data.hpp>
#include <preview-system/shader/shader.hpp>
#include <preview-system/shader/shader-files.hpp>

namespace racer
{
    class RealtimeRendererSystem
    {
        previewSystem::PrimitiveModel *sphereModel;
        previewSystem::PrimitiveModel *cubeModel;

        previewSystem::Shader shader;

        int width_, height_;

        void setPrimitiveModels()
        {
            sphereModel = new previewSystem::PrimitiveModel(previewSystem::sphereObjDataFile);
            cubeModel = new previewSystem::PrimitiveModel(previewSystem::cubeObjDataFile);
        }

    public:
        unsigned int framebuffer;
        unsigned int textureColorbuffer;
        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        unsigned int rbo;

        void ResizeTexture(int width, int height)
        {
            glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.

            width_ = width;
            height_ = height;

            glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        }

        RealtimeRendererSystem(int width = 1024, int height = 720)
            : shader(previewSystem::MVP_VertexShader, previewSystem::diffuseFragmentShader)
        {
            width_ = width;
            height_ = height;

            setPrimitiveModels();

            // framebuffer configuration
            // -------------------------
            glGenFramebuffers(1, &framebuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right

            // create a color attachment texture
            glGenTextures(1, &textureColorbuffer);
            glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);                   // use a single renderbuffer object for both a depth AND stencil buffer.
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
            // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                throw "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        ~RealtimeRendererSystem()
        {
            delete sphereModel;
            delete cubeModel;
        }

        void RenderScene(Scene *scene)
        {
            // bind to framebuffer and draw scene as we normally would to color texture
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

            glClearColor(scene->environmentColor.r, scene->environmentColor.g, scene->environmentColor.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            for (int i = 0; i < scene->shapesToRender.size(); i++)
            {
                shader.use();

                glm::mat4 projection = glm::perspective((scene->activeCamera->GetVFov((float)width_ / (float)height_)), ((float)width_ / (float)height_), 0.1f, 100.0f);

                // camera/view transformation
                glm::mat4 view = glm::lookAt(scene->activeCamera->holdingEntity->transform->position,
                                             scene->activeCamera->holdingEntity->transform->position + glm::vec3(0, 0, -1),
                                             glm::vec3(0, 1, 0));

                // calculate the model matrix for each object and pass it to shader before drawing
                glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                model = glm::translate(model, scene->shapesToRender[i]->holdingEntity->transform->position);
                model = glm::scale(model, scene->shapesToRender[i]->holdingEntity->transform->scale);

                shader.setVec3("viewDir", glm::vec3(0, 0, -1));

                shader.setVec3("color", scene->shapesToRender[i]->renderingMaterial.color);

                shader.setMat4("MVP", projection * view * model);

                sphereModel->Render();
            }

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            for (int i = 0; i < scene->shapesToRender.size(); i++)
            {
                shader.use();

                glm::mat4 projection = glm::perspective((scene->activeCamera->GetVFov((float)width_ / (float)height_)), ((float)width_ / (float)height_), 0.1f, 100.0f);

                // camera/view transformation
                glm::mat4 view = glm::lookAt(scene->activeCamera->holdingEntity->transform->position,
                                             scene->activeCamera->holdingEntity->transform->position + glm::vec3(0, 0, -1),
                                             glm::vec3(0, 1, 0));

                // calculate the model matrix for each object and pass it to shader before drawing
                glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                model = glm::translate(model, scene->shapesToRender[i]->holdingEntity->transform->position);
                model = glm::scale(model, scene->shapesToRender[i]->holdingEntity->transform->scale * 1.001f);

                shader.setVec3("viewDir", glm::vec3(0, 0, -1));

                shader.setVec3("color", glm::vec3(0.0));

                shader.setMat4("MVP", projection * view * model);

                sphereModel->Render();
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    };
}

#endif // RACER_SYSTEMS_REALTIMERENDERER_H_