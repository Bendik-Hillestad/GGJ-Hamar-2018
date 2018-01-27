#include "Window.h"

namespace GGJ
{
    int Window::GetWidth() const noexcept
    {
        return this->width;
    }

    int Window::GetHeight() const noexcept
    {
        return this->height;
    }
};
