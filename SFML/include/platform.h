#include <SFML/Graphics.hpp>
#include <cstdlib>  // Dodaj ten nagłówek dla funkcji rand()

class Platform {
private:
    sf::Texture texture;  // Dodaj obiekt tekstury
    sf::Vector2f position;

public:
    Platform(float x, float y, float width, float height)
    {
        position = {x, y};

        // Ustawienie losowego sprite'a
        int randomTexture = rand() % 3 + 1;  // Zakładamy, że masz trzy różne tekstury
        if (texture.loadFromFile("..\\textures\\platform" + std::to_string(randomTexture) + ".png")) {
            // Pamiętaj, że tekstury mogą mieć inną wielkość niż podane width i height
        }
    }

    // Sprawdza kolizję z postacią
    bool isColliding(const sf::RectangleShape& hero) const
    {
        sf::FloatRect platformBounds(position.x, position.y, texture.getSize().x, texture.getSize().y);
        return platformBounds.intersects(hero.getGlobalBounds());
    }

    // Rysuje platformę na ekranie
    void draw(sf::RenderWindow& window) const
    {
        sf::Sprite sprite(texture);
        sprite.setPosition(position);
        window.draw(sprite);
    }

    // Sprawdza pozycje platformy
    sf::Vector2f getPosition() const
    {
        return position;
    }
};