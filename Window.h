#ifndef WINDOW_H
#define WINDOW_H
#pragma once

namespace GGJ
{
    struct window_state_t;

    class Window final
    {
    public:
        static Window* InitWindow(int width, int height) noexcept;

        bool InitGL () noexcept;
        bool Poll   () noexcept;
        void Show   () noexcept;
        void Destroy() noexcept;

        Window(Window const &) = delete;
        Window(Window &&)      = delete;

    private:
        Window() {};

        int width;
        int height;
        int refreshRate;

        window_state_t* data = nullptr;
    };
};

#endif
