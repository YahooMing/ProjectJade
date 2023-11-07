#include <SFML/Graphics.hpp>

class Platform {
private:
    sf::RectangleShape shape;

public:
    Platform(float x, float y, float width, float height)
    {
        shape.setSize({width, height});
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Blue); 
    }

    // Sprawdza kolizję z postacią
    bool isColliding(const sf::RectangleShape& hero) const
    {
        return shape.getGlobalBounds().intersects(hero.getGlobalBounds());
    }

    // Rysuje platformę na ekranie
    void draw(sf::RenderWindow& window) const
    {
        window.draw(shape);
    }

    // Sprawdza pozycje platformy
    sf::Vector2f getPosition() const
    {
        return shape.getPosition();
    }
};