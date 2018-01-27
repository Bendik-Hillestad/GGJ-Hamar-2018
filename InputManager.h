#ifndef GGJ_2018_INPUT_MANAGER_H
#define GGJ_2018_INPUT_MANAGER_H
#pragma once

#include <queue>

namespace GGJ
{
    struct input_event_t;

    enum Key
    {
        NONE     =  0,

        Pressed  =  1,
        Released =  2,

        W        =  4,
        A        =  8,
        S        = 12,
        D        = 16,

        One      = 20,
        Two      = 24,
        Three    = 28,

        Z        = 32,
    };

    class InputManager final
    {
    public:
        static InputManager* GetInputManager() noexcept;

        void PushEvent(input_event_t* event) noexcept;
        Key  Poll     ()                     noexcept;

        InputManager(InputManager const &) = delete;
        InputManager(InputManager &&)      = delete;

        friend class Window;

    private:
        InputManager() noexcept {};

        std::queue<Key> inputQueue;
    };
};

#endif
