#include <SFML/Graphics.hpp>
#include "platform.h"
#include <iostream>
#include "enemy.h"
#include <vector>


enum GameState{
    MainMenu,
    Gameplay,
    LevelSelection,
    Exit
};

class MenuItem {
public:
    sf::Text text;
    bool isSelected;
    MenuItem(const std::string& content, const sf::Font& font, unsigned int characterSize, sf::Color color, sf::Vector2f position)
        : isSelected(false) {
        text.setString(content);
        text.setFont(font);
        text.setCharacterSize(characterSize);
        text.setFillColor(color);
        text.setPosition(position);
    }
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
    sf::Color invisible(0, 0, 0, 0);
    hero.setFillColor(invisible);

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
    bool isGamePaused = false;

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
    startText.setString("START (ENTER)");

    sf::Text exitText;
    exitText.setFont(font);
    exitText.setCharacterSize(50);
    exitText.setFillColor(sf::Color::White);
    exitText.setString("WYJDZ (ESC)");

    sf::Texture mcTexture;
    if (!mcTexture.loadFromFile("mc.png")) {
        return -1;
    }
    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("enemy.png")) {
        return -1;
    }

    //sf::IntRect rectSourceSprite(0, 0, 32, 32); // Początkowy obszar źródłowy (rozmiar klatki)
    // Utwórz obiekt sprite
    sf::Sprite MC(mcTexture);
    MC.setScale(5, 5); // Ustaw skalę sprite'a

    sf::Sprite enemy(enemyTexture);
    enemy.setScale(4, 4);

    startText.setPosition(window.getSize().x / 2.0f - startText.getLocalBounds().width / 2.0f,
                       window.getSize().y / 2.0f - startText.getLocalBounds().height / 2.0f);

    exitText.setPosition(window.getSize().x / 2.0f - exitText.getLocalBounds().width / 2.0f,
                       window.getSize().y / 2.0f - exitText.getLocalBounds().height / 2.0f + startText.getLocalBounds().height);

    sf::Text endText;
    endText.setFont(font);
    endText.setCharacterSize(50);
    endText.setFillColor(sf::Color::White);
    endText.setString("KONIEC GRY");

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
                }else if(event.key.code == sf::Keyboard::Escape){
                    if(gameState == MainMenu){
                        window.close();
                    }
                }else if (event.key.code == sf::Keyboard::Tab && gameState == Gameplay ){
                isGamePaused = !isGamePaused;
            }
            }
        }
       
        if(gameState == MainMenu){
            window.clear();
            window.draw(startText);
            window.draw(exitText);
            window.display();
        }
        else if (gameState == Gameplay && !isGamePaused){
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
        MC.setPosition(hero.getPosition().x, hero.getPosition().y-10); // Ustaw pozycję sprite'a na pozycji bohatera
        enemy.setPosition(enemies[0].getPosition().x, enemies[0].getPosition().y-10);


        window.clear();
        window.draw(hero);
        window.draw(enemy);
        window.draw(MC);
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

            // Pozostawienie bohatera na swojej pozycji bez dostosowywania
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
        }else if (isGamePaused)  // Dodaj warunek sprawdzający, czy gra jest zatrzymana
    {
        // Wyświetl napis "GRA ZATRZYMANA"
        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setCharacterSize(50);
        pauseText.setFillColor(sf::Color::White);
        pauseText.setString("GRA ZATRZYMANA");
        pauseText.setPosition(camera.getCenter().x, camera.getCenter().y);
        window.clear();
        window.draw(pauseText);
        window.display();
    }
   
    }

    return 0;
}