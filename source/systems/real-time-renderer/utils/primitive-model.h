#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <tiny_obj_loader.h>

namespace racer
{
    class PrimitiveModel
    {
        GLuint modelVertexArrayObject;
        GLuint indicesSize;
        // std::vector<unsigned int> modelIndices;

    public:
        PrimitiveModel(std::string modelDataFile)
        {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;

            std::string warn, err;

            std::istringstream dataStringStream(modelDataFile);

            std::vector<Vertex> vertices;

            if (tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &dataStringStream, NULL, true, true))
            {
                for (int s = 0; s < shapes.size(); s++)
                {
                    // Loop over faces(polygon)
                    indicesSize = shapes[s].mesh.indices.size();
                    int index_offset = 0;
                    for (int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
                    {
                        int fv = shapes[s].mesh.num_face_vertices[f];

                        // Loop over vertices in the face.
                        for (int v = 0; v < fv; v++)
                        {
                            Vertex vertex;

                            // access to vertex
                            tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                            vertex.Position.x = attrib.vertices[3 * int(idx.vertex_index) + 0];
                            vertex.Position.y = attrib.vertices[3 * int(idx.vertex_index) + 1];
                            vertex.Position.z = attrib.vertices[3 * int(idx.vertex_index) + 2];

                            // Check if `normal_index` is zero or positive. negative = no normal data
                            if (idx.normal_index >= 0)
                            {
                                vertex.Normal.x = attrib.normals[3 * int(idx.normal_index) + 0];
                                vertex.Normal.y = attrib.normals[3 * int(idx.normal_index) + 1];
                                vertex.Normal.z = attrib.normals[3 * int(idx.normal_index) + 2];
                            }

                            // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                            if (idx.texcoord_index >= 0)
                            {
                                vertex.TexCoords.x = attrib.texcoords[2 * int(idx.texcoord_index) + 0];
                                vertex.TexCoords.y = attrib.texcoords[2 * int(idx.texcoord_index) + 1];
                            }

                            vertices.push_back(vertex);
                        }
                        index_offset += fv;
                    }
                }

                GLuint cubeVertexBufferObject, cubeElementBufferObject;

                glGenVertexArrays(1, &modelVertexArrayObject);
                glGenBuffers(1, &cubeVertexBufferObject);
                glGenBuffers(1, &cubeElementBufferObject);

                glBindVertexArray(modelVertexArrayObject);
                glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBufferObject);

                glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

                // vertex positions
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
                // vertex normals
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
                // vertex texture coords
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

                glBindVertexArray(0);
            }
            else
            {
                std::cout << "Error loading model\n";
            }
        }

        void Render()
        {
            glBindVertexArray(modelVertexArrayObject);
            glDrawArrays(GL_TRIANGLES, 0, indicesSize);
            glBindVertexArray(0);
        }
    };
}