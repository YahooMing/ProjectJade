#include <SFML/Graphics.hpp>

int main()
{
    sf::VideoMode rozdzielczosc = sf::VideoMode::getDesktopMode(); //rozdzielczość

    sf::RenderWindow window(rozdzielczosc, "Project Jade");
    sf::RectangleShape hero;
    sf::RectangleShape floor;
    sf::RectangleShape platform;
    

    hero.setFillColor(sf::Color::Red);

    sf::Vector2f PozycjaStartowa(600,350);

    hero.setPosition(PozycjaStartowa);
    hero.setSize(sf::Vector2f(100, 100));

    floor.setSize(sf::Vector2f(1920, 40)); // szerokość ekranu i 40 pikseli wysokości
    floor.setFillColor(sf::Color::Green); // kolor podłogi
    
    platform.setSize(sf::Vector2f(200, 40)); // szerokość ekranu i 40 pikseli wysokości
    platform.setFillColor(sf::Color::Blue); // kolor podłogi
    platform.setPosition(100, 600);


    float xVelocity = 1;
    float yVelocity = 0; 
    const float JumpSpeed = -10.0f;
    const float groundHeight = 1000.0f;
    const float grawitacja = 0.1f; // Przyspieszenie grawitacyjne podobne do rzeczywistego
    bool isJumping = false;

    sf::Clock jumpClock;
    const float jumpCooldown = 0.7f;

    floor.setPosition(0, groundHeight);

    


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        // Poruszanie sie LEWO PRAWO
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            hero.move(-xVelocity, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            hero.move(xVelocity, 0);
        }
        
        // Skakanie
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping && jumpClock.getElapsedTime().asSeconds() >= jumpCooldown)
        {
            isJumping = true;
            yVelocity = JumpSpeed;
            jumpClock.restart();
        }
        
        // Symulacja grawitacji
        yVelocity += grawitacja;
        
        // Ruch pionowy
        hero.move(0, yVelocity);

        // Sprawdzenie, czy bohater dotarł do ziemi
        if (hero.getPosition().y + hero.getSize().y >= groundHeight)
        {
            isJumping = false;
            yVelocity = 0;
            hero.setPosition(hero.getPosition().x, groundHeight - hero.getSize().y);
        }

        window.clear();
        window.draw(hero);
        window.draw(floor);
        window.draw(platform);
        window.display();
    }

    return 0;
}
