#include <SFML/Graphics.hpp>

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