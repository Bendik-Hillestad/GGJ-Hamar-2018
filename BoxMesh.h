#ifndef GGJ_2018_BOX_MESH_H
#define GGJ_2018_BOX_MESH_H
#pragma once

#include <GL\gl3w.h>

namespace GGJ
{
    class BoxMesh final
    {
    public:
        static BoxMesh* GetBoxMesh() noexcept;

        void Bind(GLuint shaderProgram) noexcept;

    private:
        BoxMesh() noexcept {};

        GLuint vbo;
        GLuint ibo;
        bool initialised = false;
    };
};

#endif
