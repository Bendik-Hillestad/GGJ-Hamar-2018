#ifndef GGJ_2018_GAME_H
#define GGJ_2018_GAME_H
#pragma once

#include "Flick.h"

namespace GGJ
{
    class Window;

    class Game final
    {
    public:
        static Game* GetGame(char const* workingDirectory = nullptr) noexcept;

        flick_t GetWorldTime() const noexcept;

        void Run            () noexcept;
        void Cleanup        () noexcept;

        Game(Game const &) = delete;
        Game(Game &&)      = delete;

    private:
        Game(char const* workingDirectory) noexcept : workingDirectory{ workingDirectory } {};

        void Think (float dt) noexcept;
        void Render()         noexcept;

        Window* gameWindow  = nullptr;
        flick_t currentTime = flick_t{ 0 };

        char const* const workingDirectory;
    };
};

#endif
