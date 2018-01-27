#ifndef GGJ_2018_WINDOW_H
#define GGJ_2018_WINDOW_H
#pragma once

namespace GGJ
{
    struct window_state_t;

    class Window final
    {
    public:
        static Window* InitWindow(int width, int height) noexcept;

        int GetWidth () const noexcept;
        int GetHeight() const noexcept;

        bool InitGL  () noexcept;
        bool Poll    () noexcept;
        void Show    () noexcept;
        void Present () noexcept;
        void Destroy () noexcept;

        Window(Window const &) = delete;
        Window(Window &&)      = delete;

    private:
        Window() noexcept {};

        int width;
        int height;
        int refreshRate;

        window_state_t* data = nullptr;
    };
};

#endif
