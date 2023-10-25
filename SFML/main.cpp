#include <SFML/Graphics.hpp>

class Platform {
private:
    sf::RectangleShape shape;

public:
    Platform(float x, float y, float width, float height)
    {
        shape.setSize({width, height});
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Blue); 
    }

    // Sprawdza kolizję z postacią
    bool isColliding(const sf::RectangleShape& hero) const
    {
        return shape.getGlobalBounds().intersects(hero.getGlobalBounds());
    }

    // Rysuje platformę na ekranie
    void draw(sf::RenderWindow& window) const
    {
        window.draw(shape);
    }

    // Sprawdza pozycje platformy
    sf::Vector2f getPosition() const
    {
        return shape.getPosition();
    }
};

class Enemy {
private:
    sf::RectangleShape shape;
    float speed; 
    float leftBoundary;
    float rightBoundary;
    bool moveRight = true;

public:
    Enemy(float x, float y, float width, float height, float speed, float leftBound, float rightBound)
    : speed(speed), leftBoundary(leftBound), rightBoundary(rightBound)
    {
        shape.setSize({width, height});
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Magenta);
    }

    void patrol()
    {
    if (moveRight)
    {
        shape.move(speed, 0);
        if (shape.getPosition().x + shape.getSize().x > rightBoundary)
        {
            moveRight = false;
            shape.setPosition(rightBoundary - shape.getSize().x, shape.getPosition().y); // Ustawienie dokładnej pozycji przy granicy
        }
    }
    else
    {
        shape.move(-speed, 0);
        if (shape.getPosition().x < leftBoundary)
        {
            moveRight = true;
            shape.setPosition(leftBoundary, shape.getPosition().y); // Ustawienie dokładnej pozycji przy granicy
        }
    }
    }

    bool isColliding(const sf::RectangleShape& hero) const
    {
        return shape.getGlobalBounds().intersects(hero.getGlobalBounds());
    }

    void draw(sf::RenderWindow& window) const
    {
        window.draw(shape);
    }
};




int main()
{
    sf::VideoMode rozdzielczosc = sf::VideoMode::getDesktopMode(); //rozdzielczość

    sf::RenderWindow window(rozdzielczosc, "Project Jade");
    sf::RectangleShape hero;
    sf::RectangleShape floor;
    //sf::RectangleShape platform;
    std::vector<Platform> platforms;
    sf::View camera(sf::FloatRect(0, 0, rozdzielczosc.width, rozdzielczosc.height)); //stworzenie kamery


    hero.setFillColor(sf::Color::Red);

    sf::Vector2f PozycjaStartowa(600,350);

    hero.setPosition(PozycjaStartowa);
    hero.setSize(sf::Vector2f(100, 100));

    floor.setSize(sf::Vector2f(1920, 40)); 
    floor.setFillColor(sf::Color::Green); 

    std::vector<Enemy> enemies;
    
    //platform.setSize(sf::Vector2f(200, 40)); 
    //platform.setFillColor(sf::Color::Blue); 
    //platform.setPosition(100, 600);


    float xVelocity = 1;
    float yVelocity = 0; 
    const float JumpSpeed = -7.0f;
    const float groundHeight = 1000.0f;
    const float grawitacja = 0.05f; // Przyspieszenie grawitacyjne
    bool isJumping = false;
    const float xJumpVelocity = 2.0f;  // Wartość prędkości skoku w osi X


    sf::Clock jumpClock;
    const float jumpCooldown = 0.7f;

    floor.setPosition(0, groundHeight);

    platforms.push_back(Platform(100, 600, 200, 40));
    platforms.push_back(Platform(400, 500, 200, 40));

    enemies.push_back(Enemy(300, 350, 80, 80, 1.0f, 250, 500)); 



    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        camera.setCenter(hero.getPosition().x , camera.getCenter().y); //ustawienie kamery na bohaterze
        window.setView(camera); //ustawienie widoku na kamerze

        for (const auto& platform : platforms)
        {
            if (platform.isColliding(hero))
            {
                if (hero.getPosition().y + hero.getSize().y <= platform.getPosition().y + yVelocity)
                {
                hero.setPosition(hero.getPosition().x, platform.getPosition().y - hero.getSize().y);
                yVelocity = 0;
                xVelocity = 1;
                isJumping = false;
                }
            }
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))  // Skok w prawo
            {
                xVelocity = xJumpVelocity;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  // Skok w lewo
            {
                xVelocity = xJumpVelocity;
            }
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
            xVelocity = 1;
            hero.setPosition(hero.getPosition().x, groundHeight - hero.getSize().y);
        }

      
        for (const auto& enemy : enemies)
        {
            if (enemy.isColliding(hero))
            {
        // Tutaj dodaj logikę, co powinno się stać po kolizji.
        //PLACEHOLDER
        //window.close();  
            }
        }


        window.clear();
        window.draw(hero);
        window.draw(floor);  
        for (auto& enemy : enemies)
        {
            enemy.patrol();
            //enemy.chase(hero);
            enemy.draw(window);
        }

        //window.draw(platform);     
        for (const auto& platform : platforms)
        {
            platform.draw(window);
        }
        window.display();
   
    }

    return 0;
}
