#ifndef GGJ_2018_GAME_H
#define GGJ_2018_GAME_H
#pragma once

namespace GGJ
{
    class Window;

    class Game final
    {
    public:
        static Game* InitGame(char const* workingDirectory) noexcept;

        void Run    () noexcept;
        void Cleanup() noexcept;

        Game(Game const &) = delete;
        Game(Game &&)      = delete;

    private:
        Game(char const* workingDirectory) noexcept : workingDirectory{ workingDirectory } {};

        void Think(float dt) noexcept;

        Window* gameWindow = nullptr;

        char const* const workingDirectory;
    };
};

#endif
