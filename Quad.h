#ifndef GGJ_2018_QUAD_H
#define GGJ_2018_QUAD_H
#pragma once

#include <GL\gl3w.h>

namespace GGJ
{
    class Quad final
    {
    public:
        static Quad* GetQuad() noexcept;

        void Bind(GLuint shaderProgram) noexcept;

    private:
        Quad() noexcept {};

        GLuint vbo;
        GLuint ibo;
        bool initialised = false;
    };
};

#endif
