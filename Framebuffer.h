#ifndef GGJ_2018_FRAMEBUFFER_H
#define GGJ_2018_FRAMEBUFFER_H
#pragma once

#include <GL\gl3w.h>

namespace GGJ
{
    class Game;

    class Framebuffer final
    {
    public:
        Framebuffer() noexcept;

        static Framebuffer CreateBuffer(GLuint width, GLuint height, GLint magFilter = GL_LINEAR, GLint minFilter = GL_LINEAR) noexcept;
        
        void        Bind        (GLuint uniformLoc)                          noexcept;
        void        Clear       (GLfloat r, GLfloat g, GLfloat b, GLfloat a) noexcept;
        void        Dispose     ()                                           noexcept;

        GLuint      GetWidth    ()                                     const noexcept;
        GLuint      GetHeight   ()                                     const noexcept;

    private:
        friend class Game;

        GLuint texture;
        GLuint fbo;
        GLuint width;
        GLuint height;
    };
};

#endif
