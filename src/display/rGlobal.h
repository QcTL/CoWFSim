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
#include "layers/gDispLayers.h"
#include "CameraControls.h"

class rGlobal {
public:
    explicit rGlobal(std::shared_ptr<gDispLayers> gInfoL, std::shared_ptr<rPileMenus> rPMenu);

    void setUp();

    void loop();

    bool isOpen = true;

    void reloadSingularCell(int pX, int pY);

private:
    std::shared_ptr<rPileMenus> rPMenu;
    std::shared_ptr<gDispLayers> gSimL;
    sf::RenderWindow rWindow;
    sf::View rView;

    sf::VertexArray vertices;
    sf::VertexArray verSelector;
    CameraControls rCC;

    std::pair<int , int> rPosCursor;

    void reloadCellValues();
    void reloadSelValue();
};


#endif //CITYOFWEIRDFISHES_RGLOBAL_H
