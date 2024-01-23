#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Windows.h"
#include <cmath>

class Lava {
private:
    sf::RectangleShape lavaShape;
    float damage; // Obrażenia zadawane przez lawę
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f wh;
public:
    Lava(float x, float y, float width, float height, float damage)
        : damage(damage) {
        lavaShape.setPosition(x, y);
        wh = { width, height };
        lavaShape.setSize(sf::Vector2f(width, height));
        lavaShape.setFillColor(sf::Color(255, 0, 0)); // Kolor lawy (czerwony)
        texture.loadFromFile("..\\textures\\lava.jpg");
        texture.setSmooth(true);
        texture.setRepeated(true);

    }

    void draw(sf::RenderWindow& window) {
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(0, 0, wh.x, wh.y));
        sprite.setPosition(lavaShape.getPosition());
        window.draw(lavaShape);
        window.draw(sprite);
    }

    bool isColliding(const sf::RectangleShape& other) const {
        return lavaShape.getGlobalBounds().intersects(other.getGlobalBounds());
    }

    float getDamage() const {
        return damage;
    }
};