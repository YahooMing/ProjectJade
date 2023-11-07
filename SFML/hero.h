#include <iostream>

class Hero {
private:
    int health = 3; // Domyślna ilość życia bohatera
public:
    int getHealth() const {
        return health;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health <= 0) {
            // Tutaj możesz dodać logikę zakończenia gry lub inny sposób obsługi zakończenia życia bohatera.
        }
    }
};
