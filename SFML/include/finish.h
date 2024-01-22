#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

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