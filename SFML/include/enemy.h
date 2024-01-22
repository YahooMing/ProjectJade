#include <SFML/Graphics.hpp>

class Enemy {
private:
    sf::Sprite enemySprite;
    std::vector<sf::Texture> enemyRightTextures;
    std::vector<sf::Texture> enemyLeftTextures;
    int currentFrame;
    sf::Clock animationClock;
    const float animationSpeed;

    bool isMovingRight;
    float patrolRange;
    float patrolStart;
    float patrolEnd;
    float movementSpeed;

public:
    Enemy(float x, float y, float width, float height, float speed, float patrolRange)
        : animationSpeed(0.1f), isMovingRight(true), currentFrame(0), patrolRange(patrolRange),
          patrolStart(x), patrolEnd(x + patrolRange), movementSpeed(speed)
    {
        for (int i = 0; i < 2; ++i) {
            sf::Texture rightTexture;
            sf::Texture leftTexture;

            std::string filenameRight = "..\\textures\\enemy_right_" + std::to_string(i) + ".png";
            std::string filenameLeft = "..\\textures\\enemy_left_" + std::to_string(i) + ".png";

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
};