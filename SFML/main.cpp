#include <SFML/Graphics.hpp>
#include "platform.h"
#include <iostream>
#include "enemy.h"
//#include "hero.h" //do zrobienia w przyszłości

enum GameState{
    MainMenu,
    Gameplay
};

class Collectible {
public:
    sf::CircleShape shape;
    bool isCollected;

    Collectible(float x, float y) {
        shape.setRadius(20);
        shape.setFillColor(sf::Color::Yellow);
        shape.setPosition(x, y);
        isCollected = false;
    }

    void draw(sf::RenderWindow& window) {
        if (!isCollected) {
            window.draw(shape);
        }
    }

    bool isColliding(const sf::RectangleShape& other) const {
        return shape.getGlobalBounds().intersects(other.getGlobalBounds());
    }
};


int main()
{
    sf::VideoMode rozdzielczosc = sf::VideoMode::getDesktopMode(); //rozdzielczość

    sf::RenderWindow window(rozdzielczosc, "Project Jade");
    sf::RectangleShape hero;
    sf::RectangleShape floor;
    std::vector<Platform> platforms;
    sf::View camera(sf::FloatRect(0, 0, rozdzielczosc.width, rozdzielczosc.height)); //stworzenie kamery
    window.setFramerateLimit(420); //ograniczenie fps do 420
    sf::Clock clock;
    sf::Time elapsed;
    hero.setFillColor(sf::Color::Red);

    sf::Vector2f PozycjaStartowa(600,350);

    hero.setPosition(PozycjaStartowa);
    hero.setSize(sf::Vector2f(100, 100));

    floor.setSize(sf::Vector2f(10000, 40)); 
    floor.setFillColor(sf::Color::Green); 

    std::vector<Enemy> enemies;


    float xVelocity = 1;
    float yVelocity = 0; 
    const float JumpSpeed = -7.0f;
    const float groundHeight = 1000.0f;
    const float grawitacja = 0.05f; // Przyspieszenie grawitacyjne
    bool isJumping = false;
    const float xJumpVelocity = 2.0f;  // Wartość prędkości skoku w osi X
    int health = 3;
    int points = 1000;


    sf::Clock damageClock;
    const float damageCooldown = 0.5f;


    sf::Clock jumpClock;
    const float jumpCooldown = 0.7f;

    floor.setPosition(0, groundHeight);

    platforms.push_back(Platform(100, 600, 200, 40));
    platforms.push_back(Platform(400, 500, 200, 40));

    std::vector<Collectible> collectibles;

    // W miejscu, gdzie inicjalizujesz inne obiekty, dodaj kilka znajdźek:
    collectibles.push_back(Collectible(200, groundHeight - 100));
    collectibles.push_back(Collectible(600, groundHeight - 200));
    // Dodaj tyle, ile chcesz znajdźek.


    enemies.push_back(Enemy(1000, (groundHeight-80), 80, 80, 1.0f, 1000, 1500)); 

    sf::Font font;
    if (!font.loadFromFile("testfont.ttf")) {
    // Wczytaj czcionkę, którą chcesz użyć. Upewnij się, że plik czcionki znajduje się w katalogu z projektem.  
    }

    sf::Text healthText;
    healthText.setFont(font);
    healthText.setCharacterSize(24);
    healthText.setFillColor(sf::Color::White);

    sf::Text pointsText;
    pointsText.setFont(font);
    pointsText.setCharacterSize(24);
    pointsText.setFillColor(sf::Color::White);

    sf::Text startText;
    startText.setFont(font);
    startText.setCharacterSize(50);
    startText.setFillColor(sf::Color::White);
    startText.setString("Press SPACE to start");

    sf::Text endText;
    endText.setFont(font);
    endText.setCharacterSize(50);
    endText.setFillColor(sf::Color::White);
    endText.setString("GAME OVER");

    GameState gameState = MainMenu;
    bool isInMainMenu = true;
    bool isGameOver = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close(); 
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Enter){
                    if(gameState == MainMenu){
                        gameState = Gameplay;
                        isInMainMenu = false;
                    }else if(isGameOver){
                        gameState = MainMenu;
                        isGameOver = false;
                        isInMainMenu = true;
                    }
                }
            }
        }
       
        if(gameState == MainMenu){
            window.clear();
            window.draw(startText);
            window.display();
        }
        else if (gameState == Gameplay){
            if(isGameOver){
                window.clear();
                window.draw(endText);
                window.display();
            }
            elapsed = clock.restart();
        camera.setCenter(hero.getPosition().x , camera.getCenter().y); //ustawienie kamery na bohaterze
        window.setView(camera); //ustawienie widoku na kamerze
        sf::Vector2f cameraPosition = camera.getCenter();
        sf::Vector2f cameraSize = camera.getSize();
        healthText.setPosition(cameraPosition.x - cameraSize.x / 2.0f + 10, cameraPosition.y - cameraSize.y / 2.0f + 10);
        pointsText.setPosition(healthText.getPosition().x + healthText.getLocalBounds().width + 20, healthText.getPosition().y);

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
        healthText.setString("Health : " + std::to_string(health)); //wyświetlanie żyćka
        pointsText.setString("Points : " + std::to_string(points)); //wyświetlanie punktów

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


        window.clear();
        window.draw(hero);
        window.draw(floor);
        window.draw(healthText);
        window.draw(pointsText);

// Sprawdzenie kolizji z wrogami
for (auto it = enemies.begin(); it != enemies.end(); ) {
    it->patrol();
    it->draw(window);

    // Sprawdzenie kolizji z wrogiem
    if (it->isColliding(hero)) {
        if (it->isHeadHit(hero) && isJumping) {
            // Kolizja z głową wroga podczas skoku - usuń wroga i dodaj punkty
            it = enemies.erase(it);
            points += 100; // Przykładowo dodaje 100 punktów po zniszczeniu wroga
        } else {
            // Kolizja bez uderzenia w głowę - odejmij życie
            if (damageClock.getElapsedTime().asSeconds() >= damageCooldown) {
                health--;
                damageClock.restart();
            }
        }
    } else {
        ++it;
    }
}


        for (auto it = collectibles.begin(); it != collectibles.end();) {
        if (it->isColliding(hero) && !it->isCollected) {
        points += 50;  // Przykładowo przyznaj 50 punktów za zebranie każdej znajdźki
        it->isCollected = true;
    }
    it->draw(window);
    ++it;
}

        //window.draw(platform);     
        for (const auto& platform : platforms)
        {
            platform.draw(window);
        }
        window.display();
        //sf::sleep(sf::seconds(1.f/60.f));
        }
   
    }

    return 0;
}
