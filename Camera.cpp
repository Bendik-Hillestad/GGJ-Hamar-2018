#include "Camera.h"

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

    void Camera::GetViewMatrix(glm::mat4x4* out) const noexcept
    {
        *out = mat4x4   (1.0f);
        *out = translate(*out, vec3{  this->size / 2.0f, 0 });
        *out = translate(*out, vec3{ -this->pos,         0 });
        *out = ortho    (0.0f, this->size.x, 0.0f, this->size.y) * (*out);
    }
};
