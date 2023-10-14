#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace sf;

int main()
{
    Window window(VideoMode(800, 600), "OpenGL");
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    bool quit = false;
    while (!quit)
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) quit = true;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.display();
    }

    return(0);
}
