#include "Camera.h"
#include "Level.h"

#include <glm\gtc\matrix_transform.hpp>

using namespace glm;

namespace GGJ
{
    Camera::Camera(vec2 const &pos, vec2 const &size) noexcept
        : pos{ pos }, size{ size }
    {}

    void Camera::Move(glm::vec2 const &newPos) noexcept
    {
        this->pos = newPos;
    }

    void Camera::Resize(glm::vec2 const &newSize) noexcept
    {
        this->size = newSize;
    }

    void Camera::Constrain() noexcept
    {
        if (this->pos.x - this->size.x / 2.0f < 0.0f)              this->pos.x = this->size.x / 2.0f;
        if (this->pos.x + this->size.x / 2.0f > GetLevelWidth())   this->pos.x = GetLevelWidth() - this->size.x / 2.0f;
        if (this->pos.y + this->size.y / 2.0f > 0.0f)              this->pos.y = -this->size.y / 2.0f;
        if (this->pos.y - this->size.y / 2.0f < -GetLevelHeight()) this->pos.y = -GetLevelHeight() + this->size.y / 2.0f;
    }

    void Camera::GetViewMatrix(glm::mat4x4* out) const noexcept
    {
        *out = mat4x4   (1.0f);
        *out = translate(*out, vec3{  this->size / 2.0f, 0 });
        *out = translate(*out, vec3{ -this->pos,         0 });
        *out = ortho    (0.0f, this->size.x, 0.0f, this->size.y) * (*out);
    }
};
