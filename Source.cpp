#include "Game.h"

#include <cstdio>
#include <direct.h>

int main(int argc, char const* const argv[])
{
    //Get the current working directory
    char cwd[FILENAME_MAX]{};
    _getcwd(cwd, sizeof(cwd));

    //Log working directory
    std::printf("#Working directory is %s\n", cwd);

    //Initialise the game
    std::printf("#Initialising the game...\n");
    auto game = GGJ::Game::InitGame(cwd);

    //Check that it was succesful
    if (game != nullptr)
    {
        //Run the game
        std::printf("\n#Running the game...\n");
        game->Run();

        //Do the cleanup
        std::printf("\n#Game exited, performing cleanup...\n");
        game->Cleanup();
    }

    //Say goodbye
    std::printf("\n#Goodbye!");

    return 0;
}
