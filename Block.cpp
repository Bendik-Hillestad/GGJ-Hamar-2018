#include "Block.h"
#include "Window.h"
#include "Rect.h"

#include <cmath>

#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace glm;

namespace GGJ
{
    Block::Block(int x, int y, int width, int height, float invMass) noexcept : 
        x{ static_cast<float>(x) }, y{ static_cast<float>(y) },
        xvel{ 0 }, yvel{ 0 },
        width{ width }, height{ height },
        invMass{ invMass } {}

    void Block::SetPosition(float x, float y) noexcept
    {
        this->x = x;
        this->y = y;
    }

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
        model = scale    (model, vec3{ this->width / 2.0f, this->height / 2.0f, 1.0f });

        //Bind it
        glUniformMatrix4fv
        (
            glGetUniformLocation(program, "model"),
            1, false, value_ptr(model)
        );

        //Draw a quad
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    bool Block::Intersects(Block const* other) const noexcept
    {
        //Create a bounding rectanlge and check
        return Rect{ glm::vec2{this->x, this->y}, glm::vec2{ this->width / 2.0f, this->height / 2.0f} }.Intersects
              (Rect{ glm::vec2{ other->x, other->y }, glm::vec2{ other->width / 2.0f, other->height / 2.0f } });
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
