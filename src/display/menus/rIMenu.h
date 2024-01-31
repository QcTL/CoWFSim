//
// Created by Laminar on 31/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RIMENU_H
#define CITYOFWEIRDFISHES_RIMENU_H

class rIMenu {

public:
    rIMenu() {
        if (!tsTex.loadFromFile(R"(C:\Users\Robert\CityOfWeirdFishes\files\graphic\tilesets\ts_menuFont8)")) {
            std::cout << "WARNING: A TEXTURE HAS NOT LOADED PROPERLY" << std::endl;
        }
    }

    virtual void draw(sf::RenderWindow &rW) = 0;

private:
    sf::Texture tsTex;
};

#endif //CITYOFWEIRDFISHES_RIMENU_H
