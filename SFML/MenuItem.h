#include <SFML/Graphics.hpp>
//Unused
class MenuItem {
public:
    sf::Text text;
    bool isSelected;
    MenuItem(const std::string& content, const sf::Font& font, unsigned int characterSize, sf::Color color, sf::Vector2f position)
        : isSelected(false) {
        text.setString(content);
        text.setFont(font);
        text.setCharacterSize(characterSize);
        text.setFillColor(color);
        text.setPosition(position);
    }
};