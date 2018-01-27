#include "BoxMesh.h"

namespace GGJ
{
    static GLfloat const vertices[]
    {
        -1.0f,  1.0f,
         1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f
    };

    static GLuint const indices[]
    {
        0, 1, 2,
        2, 1, 3
    };

    BoxMesh* BoxMesh::GetBoxMesh() noexcept
    {
        static BoxMesh mesh{};

        //Check if it's already initialised
        if (mesh.initialised) return &mesh;

        //Ask the driver for a buffer for our vbo
        glGenBuffers(1, &mesh.vbo);

        //Bind it
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

        //Store the data
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //Ask the driver for a buffer for our ibo
        glGenBuffers(1, &mesh.ibo);

        //Bind it
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

        //Store the data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //Return the mesh
        return &mesh;
    }

    void BoxMesh::Bind(GLuint shaderProgram) noexcept
    {
        //Bind the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

        //Bind the buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);

        //Specify the vertex layout
        GLint posAttrib = glGetAttribLocation(shaderProgram, "vertexPosition");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }
};
