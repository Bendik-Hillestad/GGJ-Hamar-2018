#ifndef GGJ_2018_RECT_H
#define GGJ_2018_RECT_H
#pragma once

#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>

namespace GGJ
{
    class Rect
    {
    public:
        Rect(glm::vec2 const &center, glm::vec2 const &halfSize) noexcept;

        bool  Intersects(const Rect &rect) const noexcept;
        bool  Contains  (const Rect &rect) const noexcept;

    private:
        glm::vec2 center;
        glm::vec2 halfSize;
    };
};

#endif
