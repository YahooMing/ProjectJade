#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>

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