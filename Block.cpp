#include "Block.h"
#include "Window.h"

#include <cmath>

#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace glm;

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

    void Block::Render(GLuint program) const noexcept
    {
        //Calculate the model matrix
        mat4x4 model{ 1.0f };
        model = translate(model, vec3{ this->x, this->y, 1.0 });
        model = scale    (model, vec3{ this->width, this->height, 1.0f });

        //Bind it
        glUniformMatrix4fv
        (
            glGetUniformLocation(program, "model"),
            1, false, value_ptr(model)
        );

        //Draw a quad
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    float Block::GetPosX() const noexcept
    {
        return this->x;
    }

    float Block::GetPosY() const noexcept
    {
        return this->y;
    }

    void Block::Think(float dt) noexcept
    {}
};
