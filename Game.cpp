#include <GL\gl3w.h>

#include "Game.h"
#include "Window.h"
#include "InputManager.h"
#include "Shaders.h"
#include "Quad.h"
#include "Player.h"
#include "Flick.h"

#include <cstdio>


namespace GGJ
{
    //The length of an engine tick
    static constexpr flick_t ENGINE_TICK
    {
        std::chrono::duration_cast<flick_t>(std::chrono::duration<flick_t::rep, std::ratio<1, 60>>{1})
    };

    static Player playerBlock{ 0, 0, 32, 64, 1 };

    Game* Game::InitGame(char const* workingDirectory) noexcept
    {
        static Game game{ workingDirectory };

        //Create the window
        game.gameWindow = Window::GetWindow(800, 600);

        //Initialise OpenGL
        game.gameWindow->InitGL();

        //Build shaders
        BuildShaders();

        //Make sure the box mesh has been built
        Quad::GetQuad();

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

            //Render the frame
            this->Render();

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
        //Grab key inputs
        Key key;
        while ((key = InputManager::GetInputManager()->Poll()) != Key::NONE)
        {
            //Send to the player
            playerBlock.HandleKeyEvent(key);
        }

        //Update player
        playerBlock.Update(dt);
    }

    void Game::Render() noexcept
    {
        //Clear buffer
        glClear(GL_COLOR_BUFFER_BIT);

        //Use the shader we want
        GLuint program = UseProgram(GGJ::Program::Main);

        //Bind the box mesh
        Quad::GetQuad()->Bind(program);

        //Render the player
        playerBlock.Render(program);
    }
};