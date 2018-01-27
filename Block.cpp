#include "Block.h"
#include "Window.h"

#include <cmath>

namespace GGJ
{
    Block::Block(int x, int y, int width, int height, float invMass) noexcept : 
        x{ static_cast<float>(x) }, y{ static_cast<float>(x) },
        xvel{ 0 }, yvel{ 0 },
        width{ width }, height{ height },
        invMass{ invMass } {}

    void Block::AddAccel(float xacc, float yacc) noexcept
    {
        //Add acceleration to velocity
        this->xvel += xacc * this->invMass;
        this->yvel += yacc * this->invMass;
    }

    void Block::Update(float dt) noexcept
    {
        //Think
        this->Think(dt);

        //Update position based on velocity
        this->x += this->xvel * dt;
        this->y += this->yvel * dt;
    }

    void Block::Render(GLuint program) noexcept
    {
        //Set the scale
        glUniform2f
        (
            glGetUniformLocation(program, "scale"),
            static_cast<float>(this->width)  / Window::GetWindow()->GetWidth (),
            static_cast<float>(this->height) / Window::GetWindow()->GetHeight()
        );

        //Set the position
        glUniform2f
        (
            glGetUniformLocation(program, "pos"),
            std::floor(this->x) / Window::GetWindow()->GetWidth (),
            std::floor(this->y) / Window::GetWindow()->GetHeight()
        );

        //Draw a quad
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void Block::Think(float dt) noexcept
    {}
};
