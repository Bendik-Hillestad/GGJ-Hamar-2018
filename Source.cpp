#include "Window.h"

int main(int argc, char const* const argv[])
{
    auto* window = GGJ::Window::InitWindow(800, 600);

    window->InitGL();
    window->Show  ();

    do
    {
    } while (!window->Poll());

    return 0;
}
