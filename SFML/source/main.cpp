#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include "Windows.h"
#include "..\include\platform.h"
#include "..\include\enemy.h"
#include "..\include\Collectible.h"
#include "..\include\Lava.h"


class finish{
private:
    sf::Texture texture;  // Dodaj obiekt tekstury
    sf::Vector2f position;

public:
    finish(float x, float y){
        position = {x, y};
        texture.loadFromFile("..\\textures\\finish.png");
    }
    bool isColliding(const sf::RectangleShape& hero) const
    {
        sf::FloatRect finishBounds(position.x, position.y, texture.getSize().x, texture.getSize().y);
        return finishBounds.intersects(hero.getGlobalBounds());
    }
    void draw(sf::RenderWindow& window) const
    {
        sf::Sprite sprite(texture);
        sprite.setPosition(position);
        window.draw(sprite);
    }
    sf::Vector2f getPosition() const
    {
        return position;
    }
};

void saveScore(int score) {
    std::ofstream file("..\\textures\\scores.txt", std::ios::app); // Otwórz plik do zapisu (dopisywania)
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

    std::vector<sf::RectangleShape> bullets;
    float bulletSpeed;

    sf::Clock shootClock;
    const float shootCooldown = 1.0f;

    std::vector<int> bulletStates;

public:
    SpecialEnemy(float x, float y, float width, float height, float speed, float patrolRange, int initialHealth, float bulletSpeed)
        : animationSpeed(0.1f), isMovingRight(true), currentFrame(0), patrolRange(patrolRange),
          patrolStart(x), patrolEnd(x + patrolRange), movementSpeed(speed), health(initialHealth), bulletSpeed(bulletSpeed)
    {
        for (int i = 0; i < 2; ++i) {
            sf::Texture rightTexture;
            sf::Texture leftTexture;

            std::string filenameRight = "..\\textures\\senemy_right_" + std::to_string(i) + ".png";
            std::string filenameLeft = "..\\textures\\senemy_left_" + std::to_string(i) + ".png";

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

    const std::vector<sf::RectangleShape>& getBullets() const {
        return bullets;
    }

    int getHealth() const {
        return health;
    }

    void move(float x, float y) {
        enemySprite.setPosition(x, y);
    }

    bool isHeadJumpedOn(const sf::RectangleShape& hero) const {
        sf::FloatRect enemyBounds = enemySprite.getGlobalBounds();
        sf::FloatRect heroBounds = hero.getGlobalBounds();

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
            move(10000.0f, 10000.0f);
        }
    }

    // Nowe funkcje dla strzelania i rysowania pocisków
    void shoot() {
        sf::RectangleShape bullet(sf::Vector2f(10.0f, 5.0f));
        bullet.setPosition(enemySprite.getPosition().x, enemySprite.getPosition().y + enemySprite.getGlobalBounds().height / 2.0f);
        bullet.setFillColor(sf::Color::Red);
        if(shootClock.getElapsedTime().asSeconds() >= shootCooldown){
            bullets.push_back(bullet);
            if(isMovingRight){
                bulletStates.push_back(1);
            }else{
                bulletStates.push_back(0);
            }
            shootClock.restart();
        }
        
    }

    void updateBullets() {
        for (auto it = bullets.begin(); it != bullets.end();) {
            sf::RectangleShape& bullet = *it;
            int bulletIndex = std::distance(bullets.begin(), it);

            if (bulletStates[bulletIndex] == 1) {
                bullet.move(bulletSpeed, 0);
            } else {
                bullet.move(-bulletSpeed, 0);
            }

            // Sprawdź odległość przebytą przez pocisk
            float distanceTraveled = std::abs(bullet.getPosition().x - enemySprite.getPosition().x);
            if (distanceTraveled > (3*patrolRange)) {
                // Jeśli przekroczył pewien dystans, usuń pocisk
                it = bullets.erase(it);
                bulletStates.erase(bulletStates.begin() + bulletIndex);
            } else {
                ++it;
            }
        }
    }

    void drawBullets(sf::RenderWindow& window) const {
        for (const auto& bullet : bullets) {
            window.draw(bullet);
        }
    }

    bool isBulletColliding(const sf::RectangleShape& other) const {
    for (const auto& bullet : bullets) {
        if (bullet.getGlobalBounds().intersects(other.getGlobalBounds())) {
            return true;  // Return true if any bullet collides
        }
    }
    return false;  // Return false if no collision is found
}
};

enum GameState{
    MainMenu,
    Gameplay,
    LevelSelection,
    HallOfFame,
    Exit
};

enum ScreenMode {
    Windowed,
    Fullscreen
};

enum Level {
    Tutorial,
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
    //Ściana blokady
     sf::RectangleShape invisibleWall(sf::Vector2f(20, 1000));
    invisibleWall.setFillColor(sf::Color::Transparent); 

    sf::RectangleShape finishleWall(sf::Vector2f(20, 1000));
    finishleWall.setFillColor(sf::Color::Transparent); 


    hero.setPosition(PozycjaStartowa);
    hero.setSize(sf::Vector2f(100, 100));

    floor.setSize(sf::Vector2f((9900+1000), 100)); 
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
    bool GameFinished = false;

    sf::Clock damageClock;
    const float damageCooldown = 0.5f;

    sf::Clock jumpClock;
    const float jumpCooldown = 0.7f;

    floor.setPosition(-900, groundHeight);

    platforms.push_back(Platform(100, 600, 200, 40));
    platforms.push_back(Platform(700, 450, 200, 40));
    platforms.push_back(Platform(1200, 350, 200, 40));
    platforms.push_back(Platform(2000, 450, 200, 40));
    platforms.push_back(Platform(2800, 550, 200, 40));
    platforms.push_back(Platform(3600, 350, 200, 40));
    platforms3.push_back(Platform(400, 500, 200, 40));
    platforms.push_back(Platform(4400, 400, 200, 40));
    platforms.push_back(Platform(5200, 500, 200, 40));
    platforms.push_back(Platform(6000, 600, 200, 40));
    platforms.push_back(Platform(6800, 400, 200, 40));
    platforms.push_back(Platform(7600, 500, 200, 40));

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

    std::vector<finish> finishs;
    finishs.push_back(finish(9000, groundHeight - 50));

    // Dodaj tyle, ile chcesz znajdźek.
    std::vector<Lava> lavas;

    lavas.push_back(Lava(2000, groundHeight-1, 700, 50, 1.0f));
    lavas.push_back(Lava(4000, groundHeight-1, 200, 50, 1.0f));
    lavas.push_back(Lava(6000, groundHeight-1, 200, 50, 1.0f));
    lavas.push_back(Lava(8000, groundHeight-1, 200, 50, 1.0f));

    std::vector<SpecialEnemy> specialEnemies;

    // Add SpecialEnemies to the vector
    specialEnemies.push_back(SpecialEnemy(3000, groundHeight - 80, 80, 80, 1.0f, 300,2,3));
    specialEnemies.push_back(SpecialEnemy(5000, groundHeight - 80, 80, 80, 1.0f, 200,2,3));
    specialEnemies.push_back(SpecialEnemy(7000, groundHeight - 80, 80, 80, 1.0f, 250,2,3));

    std::vector<Enemy> enemies;

    //ZMIANA WROGA
    enemies.push_back(Enemy(1000, (groundHeight-80), 80, 80, 1.0f, 300)); 
    enemies.push_back(Enemy(1500, (groundHeight-80), 80, 80, 1.0f, 300));

    sf::Font font;
    if (!font.loadFromFile("..\\textures\\testfont.ttf")) {
        std::cout << "Nie udalo sie zaladowac czcionki! Sprawdz czy posiadasz plik testfont.ttf" << std::endl; 
        return EXIT_FAILURE;
    }

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("..\\textures\\background1.png")) {
        std::cout << "Nie udalo sie zaladowac tekstury! Sprawdz czy posiadasz plik background.png" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite backgroundsprite(backgroundTexture);

    sf::Texture backgroundTexture1;
    if (!backgroundTexture1.loadFromFile("..\\textures\\background2.png")) {
        std::cout << "Nie udalo sie zaladowac tekstury! Sprawdz czy posiadasz plik background.png" << std::endl;
        return EXIT_FAILURE;
    }

    sf::Texture backgroundTexture2;
    if (!backgroundTexture2.loadFromFile("..\\textures\\background3.png")) {
        std::cout << "Nie udalo sie zaladowac tekstury! Sprawdz czy posiadasz plik background.png" << std::endl;
        return EXIT_FAILURE;
    }


    sf::Music music1;
    if (!music1.openFromFile("..\\sounds\\music1.wav")) {
        std::cout << "Nie udalo sie zaladowac muzyki! Sprawdz czy posiadasz plik music1.wav" << std::endl;
        return EXIT_FAILURE;
    }
    music1.setVolume(20);
    music1.play();

    sf::SoundBuffer jumpBuffer;
    if (!jumpBuffer.loadFromFile("..\\sounds\\jump.wav")) {
        std::cout << "Nie udalo sie zaladowac dzwieku! Sprawdz czy posiadasz plik jump.wav" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sound jumpSound(jumpBuffer);
    jumpSound.setVolume(2);

    sf::SoundBuffer collectibleBuffer;
    if (!collectibleBuffer.loadFromFile("..\\sounds\\collectible.wav")) {
        std::cout<<"Nie udalo sie zaladowac dzwieku! Sprawdz czy posiadasz plik collectible.wav"<<std::endl;
        return EXIT_FAILURE;
    }
    sf::Sound collectibleSound(collectibleBuffer);
    collectibleSound.setVolume(2);

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

    sf::Text levelText;  
    levelText.setFont(font);
    levelText.setCharacterSize(50);
    levelText.setFillColor(sf::Color::White);
    levelText.setString("Level Selection:\n\n0. Tutorial\n1. Easy\n2. Medium\n3. Hard\n4. Hall of Fame");
    levelText.setPosition(window.getSize().x / 2.0f - levelText.getLocalBounds().width / 2.0f,
                          window.getSize().y / 2.0f - levelText.getLocalBounds().height / 2.0f);
    sf::Text menuText;
    menuText.setFont(font);
    menuText.setCharacterSize(24);
    menuText.setFillColor(sf::Color::White);
    menuText.setString("PRESS 'P' TO CHANGE SCREEN MODE");
    
    
    float margin = 10.0f;
    float xPositionMenuText = camera.getCenter().x - menuText.getLocalBounds().width - margin;
    float yPositionMenuText = margin;
    menuText.setPosition(xPositionMenuText, yPositionMenuText);

    sf::Texture mcTexture;
    if (!mcTexture.loadFromFile("..\\textures\\mc.png")) {
        std::cout<<"Nie udalo sie zaladowac tekstury! Sprawdz czy posiadasz plik mc.png"<<std::endl;
        return -1;
    }
    sf::Texture jumpR;
    if (!jumpR.loadFromFile("..\\textures\\jumpR.png")) {
        std::cout<<"Nie udalo sie zaladowac tekstury! Sprawdz czy posiadasz plik jumpR.png"<<std::endl;
        return -1;
    }
    sf::Texture jumpL;
    if (!jumpL.loadFromFile("..\\textures\\jumpL.png")) {
        std::cout<<"Nie udalo sie zaladowac tekstury! Sprawdz czy posiadasz plik jumpL.png"<<std::endl;
        return -1;
    }

    std::vector<sf::Texture> mcRight;
    std::vector<sf::Texture> mcLeft;
    const int numFrames =2;
    for(int i=0;i<numFrames;++i){
        sf::Texture texRight;
        sf::Texture texLeft;

        std::string filenameRight = "..\\textures\\right_" + std::to_string(i) + ".png";
        std::string filenameLeft = "..\\textures\\left_" + std::to_string(i) + ".png";

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


    startText.setPosition(camera.getCenter().x - startText.getLocalBounds().width / 2.0f,
                       camera.getCenter().y  - startText.getLocalBounds().height / 2.0f);

    exitText.setPosition(camera.getCenter().x  - exitText.getLocalBounds().width / 2.0f,
                       camera.getCenter().y  - exitText.getLocalBounds().height / 2.0f + startText.getLocalBounds().height);

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
    endText3.setString("PRESS ESCAPE TO GO BACK TO MAIN MENU");

    sf::Text finishedText1;
    sf::Text finishedText2;
    sf::Text finishedText3;

    finishedText1.setFont(font);
    finishedText1.setCharacterSize(50);
    finishedText1.setFillColor(sf::Color::White);
    finishedText1.setString("CONGRATULATIONS! YOU FINISHED THE GAME!");

    finishedText2.setFont(font);
    finishedText2.setCharacterSize(50);
    finishedText2.setFillColor(sf::Color::White);

    finishedText3.setFont(font);
    finishedText3.setCharacterSize(50);
    finishedText3.setFillColor(sf::Color::White);
    finishedText3.setString("PRESS ESCAPE TO CLOSE THE GAME");

    GameState gameState = MainMenu;
    bool isInMainMenu = true;
    bool isGameOver = false;

    //SAMOUCZEK ELEGANCKI:
    std::vector<Platform> tutorialPlatforms;
    std::vector<Enemy> tutorialEnemies;
    sf::Text tutorialText;

    tutorialPlatforms.push_back(Platform(200, groundHeight - 150, 200, 40));
    tutorialEnemies.push_back(Enemy(500, groundHeight - 200, 80, 80, 1.0f, 300));

    tutorialText.setFont(font);
    tutorialText.setCharacterSize(24);
    tutorialText.setFillColor(sf::Color::White);
    tutorialText.setString("PRESS SPACE TO JUMP!");
    tutorialText.setPosition(200, groundHeight - 1000);

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
                }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0) && gameState == LevelSelection) {
                    currentLevel = Tutorial;
                    gameState = Gameplay;
                    // ... Reszta kodu
                }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && gameState == LevelSelection){
                        currentLevel = Easy;
                        //std::cout << "Easy" << std::endl;
                }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && gameState == LevelSelection){
                        //std::cout << "Medium" << std::endl;
                        currentLevel = Medium;
                }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && gameState == LevelSelection){
                        currentLevel = Hard;
                        //std::cout << "Hard" << std::endl;
                    }else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && gameState == LevelSelection) {
                        gameState = HallOfFame;
                    }else if(event.key.code == sf::Keyboard::Escape){
                    if(gameState == MainMenu || GameFinished || isGameOver){
                        window.close();
                    }else if(isGamePaused){
                        gameState=MainMenu;
                        isGamePaused = false;
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
        if(GameFinished){
                finishedText1.setPosition(camera.getCenter().x - finishedText1.getLocalBounds().width/2 ,camera.getCenter().y - finishedText1.getLocalBounds().height );
                finishedText2.setPosition(camera.getCenter().x  - finishedText2.getLocalBounds().width/2 ,camera.getCenter().y  - finishedText2.getLocalBounds().height  + finishedText1.getLocalBounds().height);
                finishedText3.setPosition(camera.getCenter().x  - finishedText3.getLocalBounds().width/2 ,camera.getCenter().y - finishedText3.getLocalBounds().height  + (finishedText1.getLocalBounds().height + finishedText2.getLocalBounds().height));
                finishedText2.setString("YOUR SCORE: " + std::to_string(points));
                window.clear();
                window.draw(finishedText1);
                window.draw(finishedText2);
                window.draw(finishedText3);
                window.display();
                if(!GameSaved){
                    saveScore(points);
                    GameSaved = true;
                    }
        }else if (isGameOver){
         endText.setPosition(camera.getCenter().x - endText.getLocalBounds().width / 2.0f,
                       camera.getCenter().y - endText.getLocalBounds().height );
        endText2.setPosition(camera.getCenter().x - endText2.getLocalBounds().width / 2.0f ,
                       camera.getCenter().y  - endText2.getLocalBounds().height  + endText.getLocalBounds().height +15);
        endText3.setPosition(camera.getCenter().x - endText3.getLocalBounds().width / 2.0f,
                       camera.getCenter().y - endText3.getLocalBounds().height  + (endText.getLocalBounds().height + endText2.getLocalBounds().height +15 ));
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
        float margin = 10.0f;
        float xPositionMenuText = camera.getCenter().x- 200.0f;
        float yPositionMenuText = margin;
        menuText.setPosition(xPositionMenuText, yPositionMenuText);
        startText.setPosition(camera.getCenter().x - startText.getLocalBounds().width / 2.0f,
                       camera.getCenter().y  - startText.getLocalBounds().height / 2.0f);

        exitText.setPosition(camera.getCenter().x  - exitText.getLocalBounds().width / 2.0f,
                       camera.getCenter().y  - exitText.getLocalBounds().height / 2.0f + startText.getLocalBounds().height);
            window.clear();
            window.draw(startText);
            window.draw(exitText);
            window.draw(menuText);
            window.display();
        }else if(gameState == LevelSelection){
            levelText.setPosition(camera.getCenter().x - levelText.getLocalBounds().width / 2.0f,
                          camera.getCenter().y - levelText.getLocalBounds().height / 2.0f);
            window.clear();
            window.draw(levelText);
            window.display();
        }else if(gameState == HallOfFame){
             std::ifstream scoreFile("scores.txt");
    if (scoreFile.is_open()) {
        std::string line;
        while (std::getline(scoreFile, line)) {
            sf::Text scoreText;
            scoreText.setFont(font);
            scoreText.setCharacterSize(24);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setString(line);
            scoreText.setPosition(window.getSize().x / 2.0f - scoreText.getLocalBounds().width / 2.0f,
                                  window.getSize().y / 2.0f - scoreText.getLocalBounds().height / 2.0f);
            window.clear();
            window.draw(scoreText);
            window.display();
            sf::sleep(sf::seconds(1)); // Poczekaj sekundę przed wyświetleniem kolejnego wyniku
        }
        scoreFile.close();
    }
    gameState = MainMenu; // Po wyświetleniu wyników wróć do menu poziomu
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
            jumpSound.play();
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
        backgroundsprite.setPosition(camera.getCenter().x - cameraSize.x / 2.0f, camera.getCenter().y - cameraSize.y / 2.0f);
        
        window.draw(backgroundsprite);
        window.draw(hero);
        
        window.draw(currentMC);
        window.draw(floor);
        window.draw(healthText);
        window.draw(pointsText);

        if(currentLevel == Easy){ 
            backgroundsprite.setTexture(backgroundTexture);
            backgroundsprite.setScale(8, 8);
           invisibleWall.setPosition(90, 0); 
              window.draw(invisibleWall);
                finishleWall.setPosition(9100, 0);
                window.draw(finishleWall);
           if (hero.getGlobalBounds().intersects(invisibleWall.getGlobalBounds())){
            hero.setPosition(hero.getPosition().x + 5, hero.getPosition().y);
        }
        if (hero.getGlobalBounds().intersects(finishleWall.getGlobalBounds())){
            hero.setPosition(hero.getPosition().x - 5, hero.getPosition().y);
        }

        for(auto& finish : finishs){
            finish.draw(window);
            if(finish.isColliding(hero)){
                GameFinished = true;
            }
        }

for (auto it = specialEnemies.begin(); it != specialEnemies.end(); ) {
    it->patrol();
    it->draw(window);
    it->shoot();
    it->updateBullets();
    it->drawBullets(window);
    
    const auto& enemyBullets = it->getBullets();
    for (const auto& bullet : enemyBullets) {
        if (bullet.getGlobalBounds().intersects(hero.getGlobalBounds())) {
            if (damageClock.getElapsedTime().asSeconds() >= damageCooldown) {
            health--;
            damageClock.restart();
        }
        }
    }

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
        collectibleSound.play();
    }
    it->draw(window);
    ++it;
} 
            }else if(currentLevel == Medium){
                backgroundsprite.setTexture(backgroundTexture1);
                backgroundsprite.setScale(8, 8);
              invisibleWall.setPosition(90, 0); 
              window.draw(invisibleWall);
                finishleWall.setPosition(9100, 0);
                window.draw(finishleWall);
           if (hero.getGlobalBounds().intersects(invisibleWall.getGlobalBounds())){
            hero.setPosition(hero.getPosition().x + 5, hero.getPosition().y);
        }
        if (hero.getGlobalBounds().intersects(finishleWall.getGlobalBounds())){
            hero.setPosition(hero.getPosition().x - 5, hero.getPosition().y);
        }

        for(auto& finish : finishs){
            finish.draw(window);
            if(finish.isColliding(hero)){
                GameFinished = true;
            }
        }

for (auto it = specialEnemies.begin(); it != specialEnemies.end(); ) {
    it->patrol();
    it->draw(window);
    it->shoot();
    it->updateBullets();
    it->drawBullets(window);
    
    const auto& enemyBullets = it->getBullets();
    for (const auto& bullet : enemyBullets) {
        if (bullet.getGlobalBounds().intersects(hero.getGlobalBounds())) {
            if (damageClock.getElapsedTime().asSeconds() >= damageCooldown) {
            health--;
            damageClock.restart();
        }
        }
    }

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
        collectibleSound.play();
    }
    it->draw(window);
    ++it;
} 
            }else if(currentLevel == Hard){
                backgroundsprite.setTexture(backgroundTexture2);
            backgroundsprite.setScale(8, 7);
           invisibleWall.setPosition(90, 0); 
              window.draw(invisibleWall);
                finishleWall.setPosition(9100, 0);
                window.draw(finishleWall);
           if (hero.getGlobalBounds().intersects(invisibleWall.getGlobalBounds())){
            hero.setPosition(hero.getPosition().x + 5, hero.getPosition().y);
        }
        if (hero.getGlobalBounds().intersects(finishleWall.getGlobalBounds())){
            hero.setPosition(hero.getPosition().x - 5, hero.getPosition().y);
        }

        for(auto& finish : finishs){
            finish.draw(window);
            if(finish.isColliding(hero)){
                GameFinished = true;
            }
        }

for (auto it = specialEnemies.begin(); it != specialEnemies.end(); ) {
    it->patrol();
    it->draw(window);
    it->shoot();
    it->updateBullets();
    it->drawBullets(window);
    
    const auto& enemyBullets = it->getBullets();
    for (const auto& bullet : enemyBullets) {
        if (bullet.getGlobalBounds().intersects(hero.getGlobalBounds())) {
            if (damageClock.getElapsedTime().asSeconds() >= damageCooldown) {
            health--;
            damageClock.restart();
        }
        }
    }

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
        collectibleSound.play();
    }
    it->draw(window);
    ++it;
} 
            }else if (currentLevel == Tutorial) {
    // Kod dla poziomu samouczka
    window.clear(backgroundColorLevelOne);
    window.draw(hero);
    window.draw(currentMC);
    window.draw(floor);
    window.draw(healthText);
    window.draw(pointsText);

    // Rysuj platformy, wroga i tekst
    for (const auto& platform : tutorialPlatforms) {
        platform.draw(window);
            if (platform.isColliding(hero))
            {
                if (hero.getPosition().y + hero.getSize().y <= platform.getPosition().y + yVelocity)
                {
                tutorialText.setString("Good job! Now try to collect these coins!");
                hero.setPosition(hero.getPosition().x, platform.getPosition().y - hero.getSize().y);
                yVelocity = 0;
                xVelocity = 1;
                isJumping = false;
                }
            }
    }

    for (auto it = tutorialEnemies.begin(); it != tutorialEnemies.end();) {
        it->patrol();
        it->draw(window);

        // Sprawdź kolizję z wrogiem
        if (it->isColliding(hero) && it->isHeadJumpedOn(hero) && isJumping) {
            // Obsłuż kolizję z wrogiem (eliminacja wroga)
            it->move(10000.0f, 10000.0f);  // Przesuń wroga poza widoczność
            tutorialText.setString("Bravo! Jumping on enemy's head eliminates it! It's so easy, isn't it? Try Jumping on that platform over here!");
        } else {
            ++it;
        }
    }

    // Sprawdź, czy gracz naciska spację
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping && jumpClock.getElapsedTime().asSeconds() >= jumpCooldown) {
        isJumping = true;
        yVelocity = JumpSpeed;
        jumpClock.restart();
    }

    window.draw(tutorialText);
    window.display();
}
        window.display();
        }else if (isGamePaused){
        // Wyświetl napis "GRA ZATRZYMANA"
        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setCharacterSize(50);
        pauseText.setFillColor(sf::Color::White);
        pauseText.setString("GAME PAUSED\nPRESS TAB TO RESUME\nPRESS ESCAPE TO GO BACK TO EXIT THE GAME");
        pauseText.setPosition(camera.getCenter().x-pauseText.getLocalBounds().width/2.0f, camera.getCenter().y);
        window.clear();
        window.draw(pauseText);
        window.display();
    }
    }

    return 0;
}