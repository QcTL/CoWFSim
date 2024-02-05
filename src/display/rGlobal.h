//
// Created by Laminar on 29/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RGLOBAL_H
#define CITYOFWEIRDFISHES_RGLOBAL_H


#include <memory>
#include <utility>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "rPileMenus.h"
#include "layers/gSimLayers.h"

class rGlobal {
public:
        rGlobal(std::shared_ptr<gSimLayers> gInfoL);

        void setUp();
        void loop();

        bool isOpen = true;
private:
    std::unique_ptr<rPileMenus> rPMenu;
    std::shared_ptr<gSimLayers> gSimL;
    sf::RenderWindow rWindow;
    sf::View rView;
    sf::VertexArray vertices;
};


#endif //CITYOFWEIRDFISHES_RGLOBAL_H