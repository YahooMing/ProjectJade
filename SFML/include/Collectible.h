#include <SFML/Graphics.hpp>

class Collectible {
public:
    sf::Texture texture;
    bool isCollected;
    sf::Vector2f position;
    sf::Sprite sprite;

    Collectible(float x, float y) {
        position = {x, y};
        isCollected = false;
        texture.loadFromFile("..\\textures\\coin.png");
        texture.setSmooth(true);
    }

    void draw(sf::RenderWindow& window) {
        if (!isCollected) {
        sprite.setTexture(texture);
        sprite.setPosition(position);
        window.draw(sprite);
        }
    }

    bool isColliding(const sf::RectangleShape& other) const {
        return sprite.getGlobalBounds().intersects(other.getGlobalBounds());
    }
};