#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Windows.h"
#include <cmath>

class Lava {
private:
    sf::RectangleShape lavaShape;
    float damage; // Obrażenia zadawane przez lawę
public:
    Lava(float x, float y, float width, float height, float damage)
        : damage(damage) {
        lavaShape.setPosition(x, y);
        lavaShape.setSize(sf::Vector2f(width, height));
        lavaShape.setFillColor(sf::Color(255, 0, 0)); // Kolor lawy (czerwony)
    }

    void draw(sf::RenderWindow& window) {
        window.draw(lavaShape);
    }

    bool isColliding(const sf::RectangleShape& other) const {
        return lavaShape.getGlobalBounds().intersects(other.getGlobalBounds());
    }

    float getDamage() const {
        return damage;
    }
};