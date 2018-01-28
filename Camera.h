#ifndef GGJ_2018_CAMERA_H
#define GGJ_2018_CAMERA_H
#pragma once

#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>

namespace GGJ
{
    class Camera final
    {
    public:
        Camera(glm::vec2 const &pos, glm::vec2 const &size) noexcept;

        void Move         (glm::vec2 const &newPos)  noexcept;
        void Resize       (glm::vec2 const &newSize) noexcept;
        void Constrain    ()                         noexcept;
        
        void GetViewMatrix(glm::mat4x4* out) const noexcept;

    private:
        glm::vec2 pos;
        glm::vec2 size;
    };
};

#endif
