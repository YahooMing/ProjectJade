#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Windows.h"
#include <cmath>


class Boss {
private:
    sf::Sprite bossSprite;
    std::vector<sf::Texture> bossRightTextures;
    std::vector<sf::Texture> bossLeftTextures;
    int currentFrame;
    sf::Clock animationClock;
    const float animationSpeed;
    sf::Sprite playerSprite;

    bool isMovingRight;
    float movementSpeed;
    float detectionRange;
    bool isChasingPlayer;

    sf::Clock shootClock;
    const float shootCooldown;
    float bulletSpeed;
    std::vector<sf::RectangleShape> bullets;

    int health;
    const int maxHealth;

public:
    Boss(float x, float y, float width, float height, float speed, float detectionRange, int maxHealth)
        : animationSpeed(0.1f), isMovingRight(true), currentFrame(0),
          movementSpeed(speed), detectionRange(detectionRange), isChasingPlayer(false),
          shootCooldown(2.0f), bulletSpeed(5.0f), health(maxHealth), maxHealth(maxHealth)
    {
        for (int i = 0; i < 2; ++i) {
            sf::Texture rightTexture;
            sf::Texture leftTexture;

            std::string filenameRight = "enemy_right_" + std::to_string(i) + ".png";
            std::string filenameLeft = "enemy_left_" + std::to_string(i) + ".png";

            if (!rightTexture.loadFromFile(filenameRight) || !leftTexture.loadFromFile(filenameLeft)) {
                // Handle error
            }

            bossRightTextures.push_back(rightTexture);
            bossLeftTextures.push_back(leftTexture);
        }

        bossSprite.setTexture(bossRightTextures[0]);
        bossSprite.setPosition(x, y);
        bossSprite.setScale(4, 4);
        bossSprite.setOrigin(width / 2, height / 2);  // Set the origin to the center for rotation
    }

    void update(const sf::Vector2f& playerPosition) {
        float distanceToPlayer = std::abs(playerPosition.x - bossSprite.getPosition().x);

        // Check if player is within detection range
        if (distanceToPlayer < detectionRange) {
            isChasingPlayer = true;

            // Move towards the player
            if (playerPosition.x > bossSprite.getPosition().x) {
                bossSprite.move(movementSpeed, 0);
                isMovingRight = true;
            } else {
                bossSprite.move(-movementSpeed, 0);
                isMovingRight = false;
            }
        } else {
            isChasingPlayer = false;
        }

        // Animation logic
        if (animationClock.getElapsedTime().asSeconds() >= animationSpeed) {
            currentFrame = (currentFrame + 1) % 2;
            animationClock.restart();
        }

        // Update sprite based on direction
        if (isMovingRight) {
            bossSprite.setTexture(bossRightTextures[currentFrame]);
        } else {
            bossSprite.setTexture(bossLeftTextures[currentFrame]);
        }

        // Shooting logic
        if (isChasingPlayer && shootClock.getElapsedTime().asSeconds() >= shootCooldown) {
            shootBullet(playerPosition);
            shootClock.restart();
        }

        // Update bullets
        for (auto& bullet : bullets) {
            bullet.move((isMovingRight ? bulletSpeed : -bulletSpeed), 0);

            // Check collision with player
            if (bullet.getGlobalBounds().intersects(playerSprite.getGlobalBounds())) {
                // Handle player hit, e.g., decrease player health
            }
        }

        // Remove bullets that are out of screen
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const sf::RectangleShape& bullet) {
                return bullet.getPosition().x < 0 || bullet.getPosition().x > 1000;
            }), bullets.end());
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(bossSprite);

        // Draw bullets
        for (const auto& bullet : bullets) {
            window.draw(bullet);
        }
    }

    bool isChasing() const {
        return isChasingPlayer;
    }

    bool isHitOnHead(const sf::RectangleShape& player) const {
        // Check if player is above the boss and within the width of the boss
        return player.getPosition().y + player.getSize().y < bossSprite.getPosition().y &&
               player.getPosition().x + player.getSize().x > bossSprite.getPosition().x - bossSprite.getGlobalBounds().width / 2 &&
               player.getPosition().x < bossSprite.getPosition().x + bossSprite.getGlobalBounds().width / 2;
    }

    void takeDamage() {
        health--;

        if (health <= 0) {
            // Boss defeated, implement your logic here
        }
    }

private:
    void shootBullet(const sf::Vector2f& playerPosition) {
        sf::RectangleShape bullet(sf::Vector2f(10, 5));
        bullet.setFillColor(sf::Color::Red);
        bullet.setPosition(bossSprite.getPosition().x, bossSprite.getPosition().y);
        bullets.push_back(bullet);
    }
};
