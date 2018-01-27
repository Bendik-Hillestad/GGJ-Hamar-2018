#ifndef GGJ_2018_FRAMEBUFFER_H
#define GGJ_2018_FRAMEBUFFER_H
#pragma once

#include <GL\gl3w.h>

namespace GGJ
{
    class Framebuffer final
    {
    public:
        static Framebuffer CreateBuffer(GLuint width, GLuint height, GLint magFilter = GL_NEAREST, GLint minFilter = GL_NEAREST) noexcept;
        
        void        Bind        (GLuint offset) noexcept;
        void        Dispose     ()              noexcept;

        GLuint      GetWidth    ()              const noexcept;
        GLuint      GetHeight   ()              const noexcept;

    private:
        Framebuffer() noexcept;

        GLuint texture;
        GLuint width;
        GLuint height;
    };
};

#endif
