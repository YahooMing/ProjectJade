#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "platform.h"
#include <iostream>
#include "enemy.h"
#include <vector>
#include "Collectible.h"
#include "Windows.h"
#include <cmath>
#include "Lava.h"
#include <fstream>
#include <ctime>
//#include "boss.h"

void saveScore(int score) {
    std::ofstream file("scores.txt", std::ios::app); // Otwórz plik do zapisu (dopisywania)
    if (file.is_open()) {
        // Pobierz aktualną datę i czas
        std::time_t currentTime = std::time(nullptr);
        std::tm* localTime = std::localtime(&currentTime);

        // Zapisz punkty i datę do pliku
        file << "PUNKTACJA: " << score << " - DATA: " << (localTime->tm_year + 1900) << "-"
             << (localTime->tm_mon + 1) << "-" << localTime->tm_mday << " "
             << localTime->tm_hour << ":" << localTime->tm_min << ":" << localTime->tm_sec << std::endl;

        file.close(); // Zamknij plik
    } else {
        std::cerr << "Nie udało się otworzyć pliku do zapisu!" << std::endl;
    }
}

class SpecialEnemy {
private:
    sf::Sprite enemySprite;
    std::vector<sf::Texture> enemyRightTextures;
    std::vector<sf::Texture> enemyLeftTextures;
    int currentFrame;
    sf::Clock animationClock;
    const float animationSpeed;
    int health;
    bool isMovingRight;
    float patrolRange;
    float patrolStart;
    float patrolEnd;
    float movementSpeed;

public:
    SpecialEnemy(float x, float y, float width, float height, float speed, float patrolRange, int initialHealth)
        : animationSpeed(0.1f), isMovingRight(true), currentFrame(0), patrolRange(patrolRange),
          patrolStart(x), patrolEnd(x + patrolRange), movementSpeed(speed), health(initialHealth)
    {
        for (int i = 0; i < 2; ++i) {
            sf::Texture rightTexture;
            sf::Texture leftTexture;

            std::string filenameRight = "senemy_right_" + std::to_string(i) + ".png";
            std::string filenameLeft = "senemy_left_" + std::to_string(i) + ".png";

            if (!rightTexture.loadFromFile(filenameRight) || !leftTexture.loadFromFile(filenameLeft)) {
                // Handle error
            }

            enemyRightTextures.push_back(rightTexture);
            enemyLeftTextures.push_back(leftTexture);
        }

        

        enemySprite.setTexture(enemyRightTextures[0]);
        enemySprite.setPosition(x, y);
        enemySprite.setScale(4, 4);
    }
    int getHealth() const {
        return health;
    }

    void move(float x, float y) {
        // Przenieś SpecialEnemy na określone współrzędne
        enemySprite.setPosition(x, y);
    }

    bool isHeadJumpedOn(const sf::RectangleShape& hero) const {
    sf::FloatRect enemyBounds = enemySprite.getGlobalBounds();
    sf::FloatRect heroBounds = hero.getGlobalBounds();

    // Sprawdź, czy bohater dotyka głowy wroga
    if (heroBounds.intersects(sf::FloatRect(enemyBounds.left + 10, enemyBounds.top, enemyBounds.width - 20, 10))) {
        return true;
    }

    return false;
}

    

    void patrol() {
        float currentX = enemySprite.getPosition().x;

        if (isMovingRight) {
            enemySprite.move(movementSpeed, 0);
            if (currentX >= patrolEnd) {
                isMovingRight = false;
            }
        } else {
            enemySprite.move(-movementSpeed, 0);
            if (currentX <= patrolStart) {
                isMovingRight = true;
            }
        }

        if (animationClock.getElapsedTime().asSeconds() >= animationSpeed) {
            currentFrame = (currentFrame + 1) % 2;
            animationClock.restart();
        }

        if (isMovingRight) {
            enemySprite.setTexture(enemyRightTextures[currentFrame]);
        } else {
            enemySprite.setTexture(enemyLeftTextures[currentFrame]);
        }
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(enemySprite);
    }

    sf::Vector2f getPosition() const {
        return enemySprite.getPosition();
    }

    bool isColliding(const sf::RectangleShape& other) const {
        return enemySprite.getGlobalBounds().intersects(other.getGlobalBounds());
    }

    void takeDamage() {
        health--;
        if (health <= 0) {
            // Tutaj można dodać kod obsługi, gdy health spadnie do zera lub poniżej
            // Na przykład, możesz przenieść wroga na inne miejsce lub zniszczyć go
            // W tym przykładzie, przenosimy wroga na pozycję (10000.0f, 10000.0f)
            move(10000.0f, 10000.0f);
        }
    }
};

enum GameState{
    MainMenu,
    Gameplay,
    LevelSelection,
    Exit
};

enum ScreenMode {
    Windowed,
    Fullscreen
};

enum Level {
    Easy,
    Medium,
    Hard,
    Nothing
};

int main()
{

    sf::Color backgroundColorLevelOne(42, 170, 138); //kolor tła
    sf::Color backgroundColorLevelTwo(42, 170, 138);
    sf::Color backgroundColorLevelThree(42, 170, 138);
    sf::Color backgroundColorMainMenu(42, 170, 138);
    ScreenMode screenMode = Windowed;
    sf::VideoMode rozdzielczosc = sf::VideoMode::getDesktopMode(); //rozdzielczość
    sf::RenderWindow window(rozdzielczosc, "Project Jade");
    sf::RectangleShape hero;
    sf::RectangleShape floor;
    std::vector<Platform> platforms;
    std::vector<Platform> platforms2;
    std::vector<Platform> platforms3;
    sf::View camera(sf::FloatRect(0, 0, rozdzielczosc.width, rozdzielczosc.height)); //stworzenie kamery
    window.setFramerateLimit(420); //ograniczenie fps do 420
    sf::Clock clock;
    sf::Time elapsed;
    sf::Color invisible(0, 0, 0, 0);
    hero.setFillColor(invisible);

    sf::Vector2f PozycjaStartowa(600,350);

    hero.setPosition(PozycjaStartowa);
    hero.setSize(sf::Vector2f(100, 100));

    floor.setSize(sf::Vector2f(1000000, 100)); 
    floor.setFillColor(sf::Color(152, 133, 88)); 

    
    Level currentLevel = Nothing;  // Poziom trudności
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
    bool GameSaved = false;

    sf::Clock damageClock;
    const float damageCooldown = 0.5f;

    sf::Clock jumpClock;
    const float jumpCooldown = 0.7f;

    floor.setPosition(-700, groundHeight);

    platforms.push_back(Platform(100, 600, 200, 40));
    platforms.push_back(Platform(400, 500, 200, 40));
    platforms.push_back(Platform(700, 450, 200, 40));
    platforms.push_back(Platform(1200, 350, 200, 40));
    platforms.push_back(Platform(1600, 550, 200, 40));
    platforms.push_back(Platform(2000, 450, 200, 40));
    platforms.push_back(Platform(2400, 350, 200, 40));
    platforms.push_back(Platform(2800, 550, 200, 40));
    platforms.push_back(Platform(3200, 450, 200, 40));
    platforms.push_back(Platform(3600, 350, 200, 40));
    platforms2.push_back(Platform(100, 600, 200, 40));
    platforms3.push_back(Platform(400, 500, 200, 40));
    platforms.push_back(Platform(4000, 500, 200, 40));
    platforms.push_back(Platform(4400, 400, 200, 40));
    platforms.push_back(Platform(4800, 600, 200, 40));
    platforms.push_back(Platform(5200, 500, 200, 40));
    platforms.push_back(Platform(5600, 400, 200, 40));
    platforms.push_back(Platform(6000, 600, 200, 40));
    platforms.push_back(Platform(6400, 500, 200, 40));
    platforms.push_back(Platform(6800, 400, 200, 40));
    platforms.push_back(Platform(7200, 600, 200, 40));
    platforms.push_back(Platform(7600, 500, 200, 40));
    platforms.push_back(Platform(8000, 400, 200, 40));

    std::vector<Collectible> collectibles;

    // W miejscu, gdzie inicjalizujesz inne obiekty, dodaj kilka znajdźek:
    collectibles.push_back(Collectible(200, groundHeight - 100));
    collectibles.push_back(Collectible(600, groundHeight - 200));
    collectibles.push_back(Collectible(800, groundHeight - 100));
    collectibles.push_back(Collectible(1000, groundHeight - 200));
    collectibles.push_back(Collectible(1400, groundHeight - 300));
    collectibles.push_back(Collectible(800, groundHeight - 100));
    collectibles.push_back(Collectible(1000, groundHeight - 200));
    collectibles.push_back(Collectible(1400, groundHeight - 300));
    collectibles.push_back(Collectible(1800, groundHeight - 150));
    collectibles.push_back(Collectible(2200, groundHeight - 250));
    collectibles.push_back(Collectible(2600, groundHeight - 200));
    collectibles.push_back(Collectible(3000, groundHeight - 100));
    collectibles.push_back(Collectible(3400, groundHeight - 300));
    collectibles.push_back(Collectible(3800, groundHeight - 150));
    collectibles.push_back(Collectible(4200, groundHeight - 250));
    collectibles.push_back(Collectible(4600, groundHeight - 200));
    collectibles.push_back(Collectible(5000, groundHeight - 100));
    collectibles.push_back(Collectible(5400, groundHeight - 300));
    collectibles.push_back(Collectible(5800, groundHeight - 150));
    collectibles.push_back(Collectible(6200, groundHeight - 250));
    collectibles.push_back(Collectible(6600, groundHeight - 200));
    collectibles.push_back(Collectible(7000, groundHeight - 100));
    collectibles.push_back(Collectible(7400, groundHeight - 300));
    collectibles.push_back(Collectible(7800, groundHeight - 150));
    collectibles.push_back(Collectible(8200, groundHeight - 250));
    collectibles.push_back(Collectible(1300, groundHeight - 450));
    collectibles.push_back(Collectible(2500, groundHeight - 400));
    collectibles.push_back(Collectible(3900, groundHeight - 450));
    collectibles.push_back(Collectible(5400, groundHeight - 350));
    collectibles.push_back(Collectible(6800, groundHeight - 400));
     collectibles.push_back(Collectible(1300, groundHeight - 550));
    collectibles.push_back(Collectible(2500, groundHeight - 500));
    collectibles.push_back(Collectible(3900, groundHeight - 550));
    collectibles.push_back(Collectible(5400, groundHeight - 450));
    collectibles.push_back(Collectible(6800, groundHeight - 500));

    // Dodaj tyle, ile chcesz znajdźek.
    std::vector<Lava> lavas;

    lavas.push_back(Lava(2000, groundHeight-1, 200, 50, 1.0f));
    lavas.push_back(Lava(4000, groundHeight, 200, 50, 1.0f));
    lavas.push_back(Lava(6000, groundHeight, 200, 50, 1.0f));
    lavas.push_back(Lava(8000, groundHeight, 200, 50, 1.0f));

    std::vector<SpecialEnemy> specialEnemies;

    // Add SpecialEnemies to the vector
    specialEnemies.push_back(SpecialEnemy(3000, groundHeight - 80, 80, 80, 1.0f, 300,2));
    specialEnemies.push_back(SpecialEnemy(5000, groundHeight - 80, 80, 80, 1.0f, 200,2));
    specialEnemies.push_back(SpecialEnemy(7000, groundHeight - 80, 80, 80, 1.0f, 250,2));

    std::vector<Enemy> enemies;

    //ZMIANA WROGA
    enemies.push_back(Enemy(1000, (groundHeight-80), 80, 80, 1.0f, 300)); 
    enemies.push_back(Enemy(1500, (groundHeight-80), 80, 80, 1.0f, 300));
    //enemies.push_back(Enemy(2000, (groundHeight-80), 80, 80, 1.0f, 1900, 2200));
    //enemies.push_back(Enemy(2500, (groundHeight-80), 80, 80, 1.0f, 2400, 2800));
   // bool enemyR1 = enemies[0].getIsEnemyRight();

    sf::Font font;
    if (!font.loadFromFile("testfont.ttf")) {
    // Wczytaj czcionkę, którą chcesz użyć. Upewnij się, że plik czcionki znajduje się w katalogu z projektem.  
    }

    sf::Music music1;
    if (!music1.openFromFile("music1.wav")) {
        // Obsługa błędu ładowania muzyki
        return EXIT_FAILURE;
    }
    music1.play();

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
    exitText.setString("EXIT (ESC)");

    sf::Text levelText;  // Tekst do wyświetlania informacji o wyborze poziomu
    levelText.setFont(font);
    levelText.setCharacterSize(24);
    levelText.setFillColor(sf::Color::White);
    levelText.setString("Level Selection:\n1. Easy\n2. Medium\n3. Hard");
    levelText.setPosition(window.getSize().x / 2.0f - levelText.getLocalBounds().width / 2.0f,
                          window.getSize().y / 2.0f - levelText.getLocalBounds().height / 2.0f);
    sf::Text menuText;
    menuText.setFont(font);
    menuText.setCharacterSize(24);
    menuText.setFillColor(sf::Color::White);
    menuText.setString("PRESS 'P' TO CHANGE SCREEN MODE");
    float xPositionMenuText = window.getSize().x - menuText.getLocalBounds().width - 20.0f;
    float yPositionMenuText = window.getSize().y - menuText.getLocalBounds().height - 20.0f;
    menuText.setPosition(xPositionMenuText, yPositionMenuText);

    //Boss boss(400, 300, 64, 64, 2.0f, 300.0f, 3);

    sf::Texture mcTexture;
    if (!mcTexture.loadFromFile("mc.png")) {
        return -1;
    }
    sf::Texture jumpR;
    if (!jumpR.loadFromFile("jumpR.png")) {
        return -1;
    }
    sf::Texture jumpL;
    if (!jumpL.loadFromFile("jumpL.png")) {
        return -1;
    }

    std::vector<sf::Texture> mcRight;
    std::vector<sf::Texture> mcLeft;
    const int numFrames =2;
    for(int i=0;i<numFrames;++i){
        sf::Texture texRight;
        sf::Texture texLeft;

        std::string filenameRight = "right_" + std::to_string(i) + ".png";
        std::string filenameLeft = "left_" + std::to_string(i) + ".png";

        if(!texRight.loadFromFile(filenameRight) || !texLeft.loadFromFile(filenameLeft)){
            return -1;
        }
        mcRight.push_back(texRight);
        mcLeft.push_back(texLeft);
    }



    // Utwórz obiekt sprite
    sf::Sprite MC(mcTexture);
    MC.setScale(5, 5); // Ustaw skalę sprite'a

    bool  isRight = false;
    bool isLeft = false;
    int currentFrame = 0;
    sf::Clock animationClock;
    const float animationSpeed = 0.1f;
    sf::Sprite currentMC = MC; // Ustaw sprite'a jako aktualny sprite


    startText.setPosition(window.getSize().x / 2.0f - startText.getLocalBounds().width / 2.0f,
                       window.getSize().y / 2.0f - startText.getLocalBounds().height / 2.0f);

    exitText.setPosition(window.getSize().x / 2.0f - exitText.getLocalBounds().width / 2.0f,
                       window.getSize().y / 2.0f - exitText.getLocalBounds().height / 2.0f + startText.getLocalBounds().height);

    sf::Text endText;
    sf::Text endText2;
    sf::Text endText3;

    endText.setFont(font);
    endText.setCharacterSize(50);
    endText.setFillColor(sf::Color::White);
    endText.setString("GAME OVER");

    endText2.setFont(font);
    endText2.setCharacterSize(50);
    endText2.setFillColor(sf::Color::White);


    endText3.setFont(font);
    endText3.setCharacterSize(50);
    endText3.setFillColor(sf::Color::White);
    endText3.setString("PRESS ESCAPE TO CLOSE THE GAME");

    GameState gameState = MainMenu;
    bool isInMainMenu = true;
    bool isGameOver = false;

    //TU DODAŁEM!!!
    int currentEnemyFrame = 0;
    sf::Clock enemyAnimationClock;
    const float enemyAnimationSpeed = 0.1f;

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
                            gameState = LevelSelection;
                            isInMainMenu = false;
                    }else if(isGameOver){
                        gameState = MainMenu;
                        isGameOver = false;
                        isInMainMenu = true;
                    }else if(gameState == LevelSelection){
                        gameState = Gameplay;
                        isGameOver = false;
                        isInMainMenu = false;
                    }
                }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && gameState == LevelSelection){
                        currentLevel = Easy;
                        //std::cout << "Easy" << std::endl;
                }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && gameState == LevelSelection){
                        //std::cout << "Medium" << std::endl;
                        currentLevel = Medium;
                }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && gameState == LevelSelection){
                        currentLevel = Hard;
                        //std::cout << "Hard" << std::endl;
                    }else if(event.key.code == sf::Keyboard::Escape){
                    if(gameState == MainMenu || isGameOver){
                        window.close();
                    }
                }else if (event.key.code == sf::Keyboard::Tab && gameState == Gameplay ){
                    isGamePaused = !isGamePaused;
                }else if (event.key.code == sf::Keyboard::P && gameState == MainMenu) {
                    // Zmiana trybu ekranu po naciśnięciu P w menu
                    if (screenMode == Windowed) {
                        screenMode = Fullscreen;
                        window.create(rozdzielczosc, "Project Jade", sf::Style::Fullscreen);
                    } else {
                        screenMode = Windowed;
                        window.create(rozdzielczosc, "Project Jade", sf::Style::Default);
                    }
                }
            }
        }
       if (isGameOver){
         endText.setPosition(camera.getCenter().x / 2.0f - endText.getLocalBounds().width / 2.0f,
                       camera.getCenter().y / 2.0f - endText.getLocalBounds().height / 2.0f);
        endText2.setPosition(camera.getCenter().x / 2.0f - endText2.getLocalBounds().width / 2.0f,
                       camera.getCenter().y / 2.0f - endText2.getLocalBounds().height / 2.0f + endText.getLocalBounds().height);
        endText3.setPosition(camera.getCenter().x / 2.0f - endText3.getLocalBounds().width / 2.0f,
                       camera.getCenter().y / 2.0f - endText3.getLocalBounds().height / 2.0f + (endText.getLocalBounds().height + endText2.getLocalBounds().height));
                endText2.setString("YOUR SCORE: " + std::to_string(points));
                window.clear();
                window.draw(endText);
                window.draw(endText2);
                window.draw(endText3);
                window.display();
                if(!GameSaved){
                    saveScore(points);
                    GameSaved = true;
                    }
       }else if(gameState == MainMenu){
            window.clear();
            window.draw(startText);
            window.draw(exitText);
            window.draw(menuText);
            window.display();
        }else if(gameState == LevelSelection){
            window.clear();
            window.draw(levelText);
            window.display();
        }else if (gameState == Gameplay && !isGamePaused){
            if (health <= 0) {
                //std::cout<<"GAME OVER"<<std::endl;
                isGameOver = true;
            }

            elapsed = clock.restart();
        camera.setCenter(hero.getPosition().x , camera.getCenter().y); //ustawienie kamery na bohaterze
        window.setView(camera); //ustawienie widoku na kamerze
        sf::Vector2f cameraPosition = camera.getCenter();
        sf::Vector2f cameraSize = camera.getSize();
        healthText.setPosition(cameraPosition.x - cameraSize.x / 2.0f + 10, cameraPosition.y - cameraSize.y / 2.0f + 10);
        pointsText.setPosition(healthText.getPosition().x + healthText.getLocalBounds().width + 20, healthText.getPosition().y);


        healthText.setString("Health : " + std::to_string(health)); //wyświetlanie żyćka
        pointsText.setString("Points : " + std::to_string(points)); //wyświetlanie punktów

        // Poruszanie sie LEWO PRAWO
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            hero.move(-xVelocity, 0);
            isLeft = true;
            isRight = false;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            hero.move(xVelocity, 0);
            isRight = true;
            isLeft = false;
        } else {
            isRight = false;
            isLeft = false;
            currentFrame = 0;
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

if (isRight || isLeft)
{
    if (animationClock.getElapsedTime().asSeconds() >= animationSpeed)
    {
        currentFrame = (currentFrame + 1) % numFrames;
        animationClock.restart();
    }
}

if (isRight)
{
    if(!isJumping){
    currentMC.setTexture(mcRight[currentFrame]);
    }else{
        currentMC.setTexture(jumpR);
    }
}
else if (isLeft)
{
    if(!isJumping){
    currentMC.setTexture(mcLeft[currentFrame]);
    }else{
        currentMC.setTexture(jumpL);
    }
}
        currentMC.setPosition(hero.getPosition().x, hero.getPosition().y-10); // Ustaw pozycję sprite'a na pozycji bohatera
        


        window.clear(backgroundColorLevelOne);
        window.draw(hero);
        
        window.draw(currentMC);
        window.draw(floor);
        window.draw(healthText);
        window.draw(pointsText);

        if(currentLevel == Easy){ 
           /* boss.update(hero.getPosition());  // Pass player's position to the boss update method
            boss.draw(window);  // Draw the boss in your game window
            if (boss.isChasing() && boss.isHitOnHead(hero)) {
            // Player successfully jumped on the boss's head
            boss.takeDamage();  // Handle the boss taking damage logic
            }
            */
for (auto it = specialEnemies.begin(); it != specialEnemies.end(); ) {
    it->patrol();
    it->draw(window);

    // Check collision with special enemy
    if (it->isColliding(hero) && it->isHeadJumpedOn(hero) && isJumping) {
        // Handle collision with special enemy
        // ...
        it->takeDamage();  // Use the returned iterator
        points += 100;
    }else if(it->isColliding(hero) && !it->isHeadJumpedOn(hero) && !isJumping){
        if (damageClock.getElapsedTime().asSeconds() >= damageCooldown) {
                health--;
                damageClock.restart();
            }
    } else {
        ++it;
        } 
}

for (auto it = enemies.begin(); it != enemies.end(); ) {
    it->patrol();
    it->draw(window);

    // Check collision with special enemy
    if (it->isColliding(hero) && it->isHeadJumpedOn(hero) && isJumping) {
        // Handle collision with special enemy
        // ...
        it->move(10000.0f, 10000.0f);  // Use the returned iterator
        points += 100;
    }else if(it->isColliding(hero) && !it->isHeadJumpedOn(hero) && !isJumping){
        if (damageClock.getElapsedTime().asSeconds() >= damageCooldown) {
                health--;
                damageClock.restart();
            }
    } else {
        ++it;
        } 
}

            for (auto& lava : lavas) {
                if (lava.isColliding(hero)) {
                    if (damageClock.getElapsedTime().asSeconds() >= damageCooldown) {
                        health -= lava.getDamage();
                        damageClock.restart();
                    }
                }
                lava.draw(window);
            }
 
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

        for (const auto& platform : platforms)
        {
            platform.draw(window);
        }

        for (auto it = collectibles.begin(); it != collectibles.end();) {
        if (it->isColliding(hero) && !it->isCollected) {
        points += 50;  // Przykładowo przyznaj 50 punktów za zebranie każdej znajdźki
        it->isCollected = true;
    }
    it->draw(window);
    ++it;
} 
            }else if(currentLevel == Medium){

        for (const auto& platform : platforms2)
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

        for (const auto& platform : platforms2)
        {
            platform.draw(window);
        }
            }else if(currentLevel == Hard){
                for (const auto& platform : platforms3)
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

        for (const auto& platform : platforms3)
        {
            platform.draw(window);
        }
            }

   

        window.display();
        }else if (isGamePaused){
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