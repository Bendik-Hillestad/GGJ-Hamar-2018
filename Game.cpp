#include <GL\gl3w.h>

#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
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
    static Camera             lightCamera{ { 0, 0 }, { 0, 0 } };
    static Player             playerBlock{ 0, 0, 32, 64, 1 };

    static std::vector<Block> gameScene{};

    static Framebuffer        occluderMap;
    static Framebuffer        shadowMap;
    static Framebuffer        lightMap;

    /* BAD GLOBAL VARIABLES ETC ETC, DON'T LOOK */

    Game* Game::GetGame(char const* workingDirectory) noexcept
    {
        static Game game{ workingDirectory };

        //Check that it has not already been initialised
        if (game.gameWindow != nullptr) return &game;

        //Create the window
        game.gameWindow = Window::GetWindow(1200, 700);

        //Initialise OpenGL
        game.gameWindow->InitGL();

        //Setup the cameras
        gameCamera.Resize(glm::vec2{ game.gameWindow->GetWidth() * 1.5f, game.gameWindow->GetHeight() * 1.5f });
        lightCamera.Resize(glm::vec2{ 800, 800 });

        //Setup framebuffers
        occluderMap = Framebuffer::CreateBuffer(512, 512);
        shadowMap   = Framebuffer::CreateBuffer(256, 1);
        lightMap    = Framebuffer::CreateBuffer(512, 512);

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

        //Resolve any collisions that might have occurred
        this->HandleCollisions();

        //Update cameras
        gameCamera.Move(glm::vec2{ playerBlock.GetPosX(), playerBlock.GetPosY() });
        lightCamera.Move(glm::vec2{ playerBlock.GetPosX(), playerBlock.GetPosY() });
    }

    void Game::HandleCollisions() noexcept
    {
        //Go through all the scene objects and test
        for (auto const &block : gameScene)
        {
            //Check for intersection
            if (playerBlock.Intersects(&block))
            {
                //Calculate movement required on the x axis
                float dx = std::max(playerBlock.x - playerBlock.width / 2.0f, block.x - block.width / 2.0f) -
                           std::min(playerBlock.x + playerBlock.width / 2.0f, block.x + block.width / 2.0f);

                //Calculate movement required on the y axis
                float dy = std::max(playerBlock.y - playerBlock.height / 2.0f, block.y - block.height / 2.0f) -
                           std::min(playerBlock.y + playerBlock.height / 2.0f, block.y + block.height / 2.0f);

                //Get the axis of least movement
                if (dx*dx < dy*dy)
                {
                    //Check which direction to move the player
                    if (playerBlock.x < block.x) playerBlock.x += dx;
                    else                         playerBlock.x -= dx;

                    playerBlock.xvel = 0;
                }
                else
                {
                    //Check which direction to move the player
                    if (playerBlock.y < block.y) playerBlock.y += dy;
                    else                         playerBlock.y -= dy;

                    playerBlock.yvel = 0;
                }
            }
        }
    }

    void Game::Render() noexcept
    {
        //Render the light
        {
            //Occlusion pass
            {
                //Get the view matrix
                glm::mat4x4 view{ 1.0f };
                lightCamera.GetViewMatrix(&view);

                //Set blend mode
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                //Limit the viewport
                glViewport(0, 0, occluderMap.width, occluderMap.height);

                //Use the occluder shader
                GLuint program = UseProgram(GGJ::Program::Occluder);
                glEnableVertexAttribArray(0);

                //Bind the box mesh
                Quad::GetQuad()->Bind(program);

                //Bind framebuffer
                glBindFramebuffer(GL_FRAMEBUFFER, occluderMap.fbo);

                //Clear buffer
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                //Bind the camera matrix
                glUniformMatrix4fv
                (
                    glGetUniformLocation(program, "view"),
                    1, false, glm::value_ptr(view)
                );

                //Render game world
                for (auto const &block : gameScene)
                {
                    block.Render(program);
                }

                //Unbind framebuffer
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
            
            //Shadow pass
            {
                //Setup matrices
                glm::mat4x4 view{ 1.0f };
                glm::mat4x4 model{ 1.0f };
                model = glm::scale(model, glm::vec3(2.0f, -2.0f, 1.0f));

                //Set blend mode
                glBlendFunc(GL_ONE, GL_ZERO);

                //Limit the viewport
                glViewport(0, 0, shadowMap.width, 1);

                //Use the shadow shader
                GLuint program = UseProgram(GGJ::Program::Shadow);
                glEnableVertexAttribArray(0);

                //Bind the box mesh
                Quad::GetQuad()->Bind(program);

                //Bind framebuffer
                glBindFramebuffer(GL_FRAMEBUFFER, shadowMap.fbo);

                //Clear buffer
                glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                //Bind occluder map
                occluderMap.Bind(glGetUniformLocation(program, "occluderMap"));

                //Set shadow resolution
                glUniform2f(glGetUniformLocation(program, "shadowRes"), shadowMap.width, 64.0f);

                //Bind the camera matrix
                glUniformMatrix4fv
                (
                    glGetUniformLocation(program, "view"),
                    1, false, glm::value_ptr(view)
                );

                //Bind the model matrix
                glUniformMatrix4fv
                (
                    glGetUniformLocation(program, "model"),
                    1, false, glm::value_ptr(model)
                );

                //Draw a quad
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //Unbind framebuffer
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            //Light pass
            {
                //Get the view matrix
                glm::mat4x4 view{ 1.0f };
                gameCamera.GetViewMatrix(&view);

                //Setup model matrix
                glm::mat4x4 model{ 1.0f };
                model = glm::translate(model, glm::vec3(playerBlock.x, playerBlock.y, 0.0f));
                model = glm::scale(model, glm::vec3(800, -800, 1.0f));

                //Set blend mode
                glBlendFunc(GL_ONE, GL_ONE);

                //Limit the viewport
                glViewport(0, 0, lightMap.width, lightMap.height);

                //Use the light shader
                GLuint program = UseProgram(GGJ::Program::Light);
                glEnableVertexAttribArray(0);

                //Bind the box mesh
                Quad::GetQuad()->Bind(program);

                //Bind framebuffer
                glBindFramebuffer(GL_FRAMEBUFFER, lightMap.fbo);

                //Clear buffer
                glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                //Bind shadow map
                shadowMap.Bind(glGetUniformLocation(program, "shadowMap"));

                //Set shadow resolution
                glUniform2f(glGetUniformLocation(program, "shadowRes"), shadowMap.width, shadowMap.height);

                //Set light color
                glUniform3f
                (
                    glGetUniformLocation(program, "lightColor"),
                    0.7f, 0.05f, 0.0f
                );

                //Bind the camera matrix
                glUniformMatrix4fv
                (
                    glGetUniformLocation(program, "view"),
                    1, false, glm::value_ptr(view)
                );

                //Bind the model matrix
                glUniformMatrix4fv
                (
                    glGetUniformLocation(program, "model"),
                    1, false, glm::value_ptr(model)
                );

                //Draw a quad
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                //Unbind framebuffer
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
        }

        //Render the scene
        {
            //Get the view matrix
            glm::mat4x4 view{ 1.0f };
            gameCamera.GetViewMatrix(&view);

            //Set blend mode
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            //Clear buffer
            glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //Setup the viewport
            glViewport(0, 0, this->gameWindow->GetWidth(), this->gameWindow->GetHeight());

            //Use the shader we want
            GLuint program = UseProgram(GGJ::Program::Main);
            glEnableVertexAttribArray(0);

            //Bind the box mesh
            Quad::GetQuad()->Bind(program);

            //Bind the camera matrix
            glUniformMatrix4fv
            (
                glGetUniformLocation(program, "view"),
                1, false, glm::value_ptr(view)
            );

            //Set the color
            glUniform3f
            (
                glGetUniformLocation(program, "color"),
                1.0f, 0.0f, 0.0f
            );

            //Render the player
            playerBlock.Render(program);

            //Set the color
            glUniform3f
            (
                glGetUniformLocation(program, "color"),
                1.0f, 1.0f, 1.0f
            );

            //Render game world
            for (auto const &block : gameScene)
            {
                block.Render(program);
            }
        }
        
        //Apply light
        {
            //Setup matrices
            glm::mat4x4 view{ 1.0f };
            glm::mat4x4 model{ 1.0f };
            model = glm::scale(model, glm::vec3(2.0f, 2.0f, 1.0f));

            //Set blend mode
            glBlendFunc(GL_DST_COLOR, GL_ZERO);

            //Use the shader we want
            GLuint program = UseProgram(GGJ::Program::Post);
            glEnableVertexAttribArray(0);

            //Bind the box mesh
            Quad::GetQuad()->Bind(program);

            //Bind light map
            lightMap.Bind(glGetUniformLocation(program, "tex"));

            //Bind the camera matrix
            glUniformMatrix4fv
            (
                glGetUniformLocation(program, "view"),
                1, false, glm::value_ptr(view)
            );

            //Bind the model matrix
            glUniformMatrix4fv
            (
                glGetUniformLocation(program, "model"),
                1, false, glm::value_ptr(model)
            );

            //Draw a quad
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
    }
};