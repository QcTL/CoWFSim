//
// Created by Laminar on 29/01/2024.
//

#include "rGlobal.h"
#include <SFML/Graphics.hpp>

rGlobal::rGlobal(std::shared_ptr<gSimLayers> gInfoL, std::shared_ptr<rPileMenus> rPMenu)
        : gSimL(std::move(gInfoL)), rPMenu(std::move(rPMenu)),
          rWindow(sf::RenderWindow(sf::VideoMode(1400, 800), "CoWF")) {


}

void rGlobal::setUp() {

    std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gSimL->gRangeUse;
    int GWidth = (gRange.first.second - gRange.first.first) + 1;
    int GHeight = (gRange.second.second - gRange.second.first) + 1;
    int GTileSize = 32; //TODO CANVIAR PER SER GENERIC;
    rPMenu->initialize(rWindow);
    rView = sf::View(sf::FloatRect(0, 0, 1400, 800));

    vertices = sf::VertexArray(sf::Quads, GWidth * GHeight * 4);
    for (int x = gRange.first.first; x <= gRange.first.second; x++) {
        for (int y = gRange.second.first; y <= gRange.second.second; y++) {

            sf::Vertex *quad = &vertices[(x + y * GWidth) * 4];

            quad[0].position = sf::Vector2f(x * GTileSize, y * GTileSize);
            quad[1].position = sf::Vector2f((x + 1) * GTileSize, y * GTileSize);
            quad[2].position = sf::Vector2f((x + 1) * GTileSize, (y + 1) * GTileSize);
            quad[3].position = sf::Vector2f(x * GTileSize, (y + 1) * GTileSize);

            for (int i = 0; i < 4; i++) {
                quad[i].texCoords = gSimL->gSLActual->getTexPos(x, y)[i];
            }

        }
    }

}

void rGlobal::loop() {
    sf::Event event;
    sf::View visibleArea;
    while (rWindow.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                rWindow.close();
                isOpen = false;
                break;
            case sf::Event::Resized:
                rView.setSize(event.size.width, event.size.height);
                rPMenu->resizedScreen(event);
                visibleArea = sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height));
                rWindow.setView(visibleArea);
                break;
        }
        rCC.updateOnEvent(event, rWindow, rView);
    }

    rCC.updateOnLoop(rView);

    rWindow.clear();
    rWindow.setView(rView);
    rWindow.draw(vertices, gSimL->gSLActual->getTexture());
    rPMenu->display(rWindow);
    rWindow.display();
}