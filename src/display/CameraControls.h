//
// Created by Laminar on 13/02/2024.
//

#ifndef CITYOFWEIRDFISHES_CAMERACONTROLS_H
#define CITYOFWEIRDFISHES_CAMERACONTROLS_H

#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class CameraControls{

public:
    CameraControls()= default;

    void updateOnEvent(const sf::Event& event, const sf::RenderWindow& rWindow, sf::View& v){
        switch (event.type) {
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    cFIsDragging = true;
                    cOldPos = sf::Mouse::getPosition(rWindow);
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    cFIsDragging = false;
                }
                break;
            case sf::Event::MouseMoved:
                if (cFIsDragging) {
                    const sf::Vector2<int> newPos = sf::Mouse::getPosition(rWindow);
                    cDelta += cOldPos - newPos;
                    cOldPos = newPos;
                }
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::R) {
                    v.setCenter(0, 0);
                }
                break;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    if (event.mouseWheelScroll.delta > 0) {
                        v.zoom(0.9f); // Zoom in
                    } else {
                        v.zoom(1.1f); // Zoom out
                    }
                }
                break;
        }
    }

    void updateOnLoop(sf::View& v){
        if (cFIsDragging) {
            sf::Vector2f zoomAdjustedDelta = sf::Vector2f(cDelta) * (v.getSize().x / 1000);
            v.move(zoomAdjustedDelta);
            cDelta = sf::Vector2i();
        }
    }

private:
    sf::Vector2<int> cDelta;
    bool cFIsDragging = false;
    sf::Vector2<int> cOldPos;
    float cZoomFactor = 1;
};
#endif //CITYOFWEIRDFISHES_CAMERACONTROLS_H
