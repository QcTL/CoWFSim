//
// Created by Laminar on 29/01/2024.
//

#include "rGlobal.h"
#include <SFML/Graphics.hpp>

rGlobal::rGlobal(std::shared_ptr<gSimLayers> gInfoL)
    : gSimL(std::move(gInfoL)), rWindow(sf::RenderWindow(sf::VideoMode(800,600), "CoWF")) {

    rPMenu = std::make_unique<rPileMenus>();
}

void rGlobal::setUp() {

    std::pair<std::pair<int,int>,std::pair<int,int>> gRange = gSimL->gSLActual->lGrid->rangeUse();
    int GWidth = (gRange.first.second - gRange.first.first)  + 1;
    int GHeight = (gRange.second.second - gRange.second.first) + 1;
    int GTileSize = 32; //TODO CANVIAR PER SER GENERIC;

    rView = sf::View(sf::FloatRect(0,0,800,600));

    vertices = sf::VertexArray(sf::Quads, GWidth * GHeight * 4);
    for (int x = gRange.first.first; x <= gRange.first.second; x++) {
        for (int y = gRange.second.first; y <= gRange.second.second; y++) {

            sf::Vertex* quad = &vertices[(x + y * GWidth) * 4];

            quad[0].position = sf::Vector2f(x * GTileSize, y * GTileSize);
            quad[1].position = sf::Vector2f((x + 1) * GTileSize, y * GTileSize);
            quad[2].position = sf::Vector2f((x + 1) * GTileSize, (y + 1) * GTileSize);
            quad[3].position = sf::Vector2f(x * GTileSize, (y + 1) * GTileSize);

            for(int i = 0; i < 4; i++){
                //TODO PRECARGAR PER NO TIRAR TOTA LA RESTA DE LECTOR CADA COP;
                quad[i].texCoords = gSimL->gSLActual->getTexPos(x,y)[i];
            }

        }
    }

}

void rGlobal::loop() {
    sf::Event event;
    while (rWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            rWindow.close();
            isOpen = false;
        }
        if (event.type == sf::Event::Resized)
        {
            rView.setSize(event.size.width, event.size.height);
        }
    }

    rWindow.clear();

    rPMenu->display(rWindow);
    rWindow.setView(rView);
    rWindow.draw(vertices, gSimL->gSLActual->getTexture());
    rWindow.display();
}