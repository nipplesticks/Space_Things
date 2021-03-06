#include "Game/Game.h"
#include <thread>
#include <time.h>

sf::Vector2f Global::g_windowSize = sf::Vector2f(1280, 720);

int main()
{
    Global::g_font.loadFromFile("AGENCYB.TTF");
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    
    sf::RenderWindow window(sf::VideoMode(1280, 720, desktop.bitsPerPixel), "Planet_stuff");
    Global::g_windowSize = (sf::Vector2f)window.getSize();

    Game game;
    game.Init();

    window.setActive(false);
    std::thread gameThread(&Game::Run, &game, &window);

    while (!game.IsRunning());

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
                game.Terminate();
            }
        }

        if (!game.IsRunning())
            window.close();
    }

    while (game.IsRunning());

    gameThread.join();

    game.Release();

    return 0;

}