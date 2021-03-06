#ifndef GGJ_2018_BLOCK_H
#define GGJ_2018_BLOCK_H
#pragma once

#include <GL\gl3w.h>

namespace GGJ
{
    class Game;

    class Block
    {
    public:
        Block(int x, int y, int width, int height, float invMass)    noexcept;

        void         SetPosition(float x, float y)                   noexcept;
        void         AddAccel   (float xacc, float yacc)             noexcept;
        void         Update     (float dt)                           noexcept;
        void         Render     (GLuint program)               const noexcept;

        bool         Intersects (Block const* other)           const noexcept;

        float        GetPosX    ()                             const noexcept;
        float        GetPosY    ()                             const noexcept;

        virtual void Think      (float dt)                           noexcept;

    protected:
        friend class Game;

        float x,      y;
        float xvel,   yvel;

        int   width,  height;

        float invMass;
    };
};

#endif
