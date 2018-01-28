#include <GL\gl3w.h>

#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Game.h"
#include "Window.h"
#include "InputManager.h"
#include "Shaders.h"
#include "Quad.h"
#include "Player.h"
#include "Camera.h"
#include "Level.h"
#include "Framebuffer.h"
#include "Flick.h"

#include <cstdio>

namespace GGJ
{
    //The length of an engine tick
    static constexpr flick_t ENGINE_TICK
    {
        std::chrono::duration_cast<flick_t>(std::chrono::duration<flick_t::rep, std::ratio<1, 60>>{1})
    };

    /* BAD GLOBAL VARIABLES ETC ETC, DON'T LOOK */
    
    static Camera             gameCamera{ { 0, 0 }, { 0, 0 } };
    static Player             playerBlock{ 0, 0, 32, 64, 1 };

    static std::vector<Block> gameScene{};

    static Framebuffer*       occluderMap = nullptr;
    static Framebuffer*       shadowMap = nullptr;
    static Framebuffer*       lightMap = nullptr;

    /* BAD GLOBAL VARIABLES ETC ETC, DON'T LOOK */

    Game* Game::GetGame(char const* workingDirectory) noexcept
    {
        static Game game{ workingDirectory };

        //Check that it has not already been initialised
        if (game.gameWindow != nullptr) return &game;

        //Create the window
        game.gameWindow = Window::GetWindow(800, 600);

        //Initialise OpenGL
        game.gameWindow->InitGL();

        //Setup the camera
        gameCamera.Resize(glm::vec2{ game.gameWindow->GetWidth(), game.gameWindow->GetHeight() });

        //Setup game world
        GGJ::LoadLevel(&playerBlock, &gameScene);

        //Build shaders
        BuildShaders();

        //Make sure the box mesh has been built
        Quad::GetQuad();

        //Show the window
        game.gameWindow->Show();

        //Return the game instance
        return &game;
    }

    flick_t Game::GetWorldTime() const noexcept
    {
        return this->currentTime;
    }

    void Game::Run() noexcept
    {
        //Start the clock
        auto old   = clock_t::now();
        auto acc   = flick_t{ 0 };

        //Enter the main game loop
        do
        {
            //Grab elapsed time
            auto now = clock_t::now();
            auto dur = std::chrono::duration_cast<flick_t>(now - old);
            old = now;

            //Accumulate time
            acc               += dur;
            this->currentTime += dur;

            //Perform engine ticks
            while (acc >= ENGINE_TICK)
            {
                //Think
                this->Think(std::chrono::duration_cast<std::chrono::duration<float>>(ENGINE_TICK).count());

                //Remove from accumulator
                acc -= ENGINE_TICK;
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

        //Update camera
        gameCamera.Move(glm::vec2{ playerBlock.GetPosX(), playerBlock.GetPosY() });

        //Make sure it nevers gets outside
        gameCamera.Constrain();
    }

    void Game::Render() noexcept
    {
        //Clear buffer
        glClear(GL_COLOR_BUFFER_BIT);

        //Use the shader we want
        GLuint program = UseProgram(GGJ::Program::Main);

        //Bind the box mesh
        Quad::GetQuad()->Bind(program);

        //Get the view matrix
        glm::mat4x4 view{ 1.0f };
        gameCamera.GetViewMatrix(&view);

        //Bind the camera matrix
        glUniformMatrix4fv
        (
            glGetUniformLocation(program, "view"),
            1, false, glm::value_ptr(view)
        );

        //Render the player
        playerBlock.Render(program);

        //Render game world
        for (auto const &block : gameScene)
        {
            block.Render(program);
        }
    }
};