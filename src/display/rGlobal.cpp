//
// Created by Laminar on 29/01/2024.
//

#include "rGlobal.h"
#include "rRemoteUpdateGrid.h"
#include <SFML/Graphics.hpp>

rGlobal::rGlobal(std::shared_ptr<gSimLayers> gInfoL, std::shared_ptr<rPileMenus> rPMenu)
        : gSimL(std::move(gInfoL)), rPMenu(std::move(rPMenu)),
          rWindow(sf::RenderWindow(sf::VideoMode(1400, 800), "CoWF")) {


}

void rGlobal::setUp() {
    //AIXO s'ha de cridar despres de cada canvii de layers...
    rPMenu->initialize(rWindow);
    rView = sf::View(sf::FloatRect(0, 0, 1400, 800));

    reloadCellValues();

    verSelector = sf::VertexArray(sf::Quads, 4);

    int GTileSize = 32;
    sf::Vertex *quad = &verSelector[0];

    quad[0].position = sf::Vector2f(0, 0);
    quad[1].position = sf::Vector2f(GTileSize, 0);
    quad[2].position = sf::Vector2f(GTileSize, GTileSize);
    quad[3].position = sf::Vector2f(0, GTileSize);

    sf::Color whiteColor = sf::Color::White;
    quad[0].texCoords = sf::Vector2f(whiteColor.r, whiteColor.g);
    quad[1].texCoords = sf::Vector2f(whiteColor.r, whiteColor.g);
    quad[2].texCoords = sf::Vector2f(whiteColor.r, whiteColor.g);
    quad[3].texCoords = sf::Vector2f(whiteColor.r, whiteColor.g);
}

void rGlobal::reloadSelValue() {

    int GTileSize = 32;
    sf::Vertex *quad = &verSelector[0];
    quad[0].position = sf::Vector2f(rPosCursor.first * GTileSize, rPosCursor.second * GTileSize);
    quad[1].position = sf::Vector2f((rPosCursor.first + 1) * GTileSize, rPosCursor.second * GTileSize);
    quad[2].position = sf::Vector2f((rPosCursor.first + 1) * GTileSize, (rPosCursor.second + 1) * GTileSize);
    quad[3].position = sf::Vector2f(rPosCursor.first * GTileSize, (rPosCursor.second + 1) * GTileSize);
}

void rGlobal::reloadCellValues() {
    std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gSimL->gRangeUse;
    int GWidth = (gRange.first.second - gRange.first.first) + 1;
    int GHeight = (gRange.second.second - gRange.second.first) + 1;
    int GTileSize = 32; //TODO CANVIAR PER SER GENERIC;
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

void rGlobal::reloadSingularCell(int pX, int pY) {
    std::pair<std::pair<int, int>, std::pair<int, int>> gRange = gSimL->gRangeUse;
    int GWidth = (gRange.first.second - gRange.first.first) + 1;

    sf::Vertex *quad = &vertices[(pX + pY * GWidth) * 4];
    for (int i = 0; i < 4; i++) {
        quad[i].texCoords = gSimL->gSLActual->getTexPos(pX, pY)[i];
    }
}

void rGlobal::loop() {
    sf::Event event{};
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
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Right) {
                    gSimL->pressedCell(rPosCursor);
                    rPMenu->pressedCell(rPosCursor);
                }
                break;
            case sf::Event::MouseMoved: {
                sf::Vector2f viewCoords = rWindow.mapPixelToCoords(sf::Mouse::getPosition(rWindow), rView);
                int xCoord = static_cast<int>(viewCoords.x / 32);
                int yCoord = static_cast<int>(viewCoords.y / 32);

                if (gSimL->gRangeUse.first.first <= xCoord && xCoord <= gSimL->gRangeUse.first.second &&
                    gSimL->gRangeUse.second.first <= yCoord && yCoord <= gSimL->gRangeUse.second.second) {
                    rPosCursor = {xCoord, yCoord};
                    reloadSelValue();
                }
            }
                break;
            default:
                break;
        }
        rCC.updateOnEvent(event, rWindow, rView);
        rPMenu->updateOnEvent(event, rWindow);
    }

    rCC.updateOnLoop(rView);
    rPMenu->updateOnLoop(rView);
    if (rRemoteUpdateGrid::hasToUpdate) {
        reloadCellValues();
        rRemoteUpdateGrid::setHasToChange(false);
    }
    rWindow.clear();
    rWindow.setView(rView);


    rWindow.draw(vertices, gSimL->gSLActual->getTexture());
    rWindow.draw(verSelector);
    rPMenu->display(rWindow);
    rWindow.display();
}