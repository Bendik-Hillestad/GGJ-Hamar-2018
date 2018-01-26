#include "Game.h"
#include "Window.h"

#include <cstdio>
#include <chrono>
#include <ratio>

namespace GGJ
{
    using flick_t = std::chrono::duration<std::chrono::nanoseconds::rep, std::ratio<1, 705600000>>;
    using clock_t = std::chrono::high_resolution_clock;

    //The length of an engine tick
    static constexpr flick_t ENGINE_TICK
    {
        std::chrono::duration_cast<flick_t>(std::chrono::duration<flick_t::rep, std::ratio<1, 60>>{1})
    };

    Game* Game::InitGame(char const* workingDirectory) noexcept
    {
        static Game game{ workingDirectory };

        //Create the window
        game.gameWindow = Window::InitWindow(800, 600);

        //Initialise OpenGL
        game.gameWindow->InitGL();

        //Show the window
        game.gameWindow->Show();

        //Return the game instance
        return &game;
    }

    void Game::Run() noexcept
    {
        //Start the clock
        auto old = clock_t::now();
        auto acc = flick_t::rep{ 0 };

        //Enter the main game loop
        do
        {
            //Grab elapsed time
            auto now = clock_t::now();
            auto dur = now - old;
            old = now;

            //Accumulate time
            acc += dur.count();

            //Perform engine ticks
            while (acc >= ENGINE_TICK.count())
            {
                //Think
                this->Think(std::chrono::duration_cast<std::chrono::duration<float>>(ENGINE_TICK).count());

                //Remove from accumulator
                acc -= ENGINE_TICK.count();
            }

            //TODO: Render the frame

            //Present the frame
            this->gameWindow->Present();
        } while (!this->gameWindow->Poll());
    }

    void Game::Cleanup() noexcept
    {
        //Cleanup the window
        this->gameWindow->Destroy();
        this->gameWindow = nullptr;
    }

    void Game::Think(float dt) noexcept
    {
        //TODO: Update the game state
        std::printf("Hello\n");
    }
};