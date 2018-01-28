#ifndef GGJ_2018_SHADERS_H
#define GGJ_2018_SHADERS_H
#pragma once

#include <GL\gl3w.h>

namespace GGJ
{
    enum Program
    {
        Main,
        Occluder,
        Shadow,
        Light,
        Post
    };

    bool   BuildShaders()                noexcept;
    GLuint UseProgram  (Program program) noexcept;
};

#endif
