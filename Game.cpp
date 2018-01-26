#include "Game.h"
#include "Window.h"

namespace GGJ
{
    Game* Game::InitGame(char const* workingDirectory) noexcept
    {
        static Game game{ workingDirectory };

        //Create the window
        game.gameWindow = Window::InitWindow(800, 600);

        game.gameWindow->InitGL();
        game.gameWindow->Show();

        //Return the game instance
        return &game;
    }

    void Game::Run() noexcept
    {
        //Enter the main game loop
        do
        {
            //TODO: Do stuff
        } while (!this->gameWindow->Poll());
    }

    void Game::Cleanup() noexcept
    {
        //Cleanup the window
        this->gameWindow->Destroy();
        this->gameWindow = nullptr;
    }
};