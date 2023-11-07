#include <SFML/Graphics.hpp>

class Enemy {
private:
    sf::RectangleShape shape;
    float speed; 
    float leftBoundary;
    float rightBoundary;
    bool moveRight = true;

public:
    Enemy(float x, float y, float width, float height, float speed, float leftBound, float rightBound)
    : speed(speed), leftBoundary(leftBound), rightBoundary(rightBound)
    {
        shape.setSize({width, height});
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Magenta);
    }

    void patrol()
    {
        if (moveRight)
        {
        shape.move(speed, 0);
        if (shape.getPosition().x + shape.getSize().x > rightBoundary)
        {
            moveRight = false;
            shape.setPosition(rightBoundary - shape.getSize().x, shape.getPosition().y);
            speed = -speed; // Zmiana kierunku prędkości po odbiciu
        }
        }
        else
        {
            shape.move(speed, 0); // Użyj wartości ujemnej prędkości, aby iść w lewo
            if (shape.getPosition().x < leftBoundary)
            {
            moveRight = true;
            shape.setPosition(leftBoundary, shape.getPosition().y);
            speed = -speed; // Zmiana kierunku prędkości po odbiciu
            }
        }
    }


    bool isColliding(const sf::RectangleShape& hero) const
    {
        return shape.getGlobalBounds().intersects(hero.getGlobalBounds());
    }

    void draw(sf::RenderWindow& window) const
    {
        window.draw(shape);
    }
};