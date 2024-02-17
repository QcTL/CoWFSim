//
// Created by Laminar on 17/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SIMINITIALIZE_H
#define CITYOFWEIRDFISHES_SIMINITIALIZE_H

#include "src/sim/structure/grids/transformation/gBaseToRiver.h"

class SimInitialize {

public:
    static int givenMap(const std::map<std::string, std::string> &mValues) {


        uint32_t lSizeGrid = 0;
        if (mValues.at("Mida_Simulacio") == "Petita") {
            lSizeGrid = 50;
        } else if (mValues.at("Mida_Simulacio") == "Mitjana") {
            lSizeGrid = 100;
        } else if (mValues.at("Mida_Simulacio") == "Gran") {
            lSizeGrid = 150;
        } else if (mValues.at("Mida_Simulacio") == "Molt_Gran") {
            lSizeGrid = 200;
        }

        std::shared_ptr<gIGrid<int>> gB = std::make_shared<gBasicGrid<int>>(gBasicGrid<int>(lSizeGrid, lSizeGrid, -1));

        std::vector<gtmElement> vElem = {
                gtmElement(0.0, 0, 0.0, 0),
                gtmElement(0.005, 0, 0.03, 0),
                gtmElement(0.15, 0, 0.3, 0),};


        std::shared_ptr<gIGrid<bool>> gUrbanCenterMask = std::make_shared<gBasicGrid<bool>>(
                gBasicGrid<bool>(lSizeGrid, lSizeGrid, true));


        std::vector<std::pair<int, int>> vPosCentre;
        if(mValues.at("Ciutat_Central") == "on")
            vPosCentre = {{lSizeGrid/2,lSizeGrid/2}, {lSizeGrid/7,lSizeGrid/3}, {lSizeGrid/7 + lSizeGrid/2,lSizeGrid/4 + lSizeGrid/2}};
        else
            vPosCentre = {{30,30}, {70,70}};
        for (int i = 0; i < std::stoi(mValues.at("Centres_Urbans")); i++) {
            gBaseToGradientMinimum gBGM(vElem, vPosCentre[i], gB, gUrbanCenterMask);
            gBGM.generateV2();
            gUrbanCenterMask = BasicTransformations::genMaskFromGrid(gB, {0});
            gB = BasicTransformations::replaceValues(gB, {{0,-1}});
        }
        gB = BasicTransformations::replaceValues(gB, {{-1,0}});


        float lSizeRiver = 0;
        if (mValues.at("Mida_Simulacio") == "Petita") {
            lSizeRiver = 1;
        } else if (mValues.at("Mida_Simulacio") == "Mitjana") {
            lSizeRiver = 1.5;
        } else if (mValues.at("Mida_Simulacio") == "Gran") {
            lSizeRiver = 2;
        } else if (mValues.at("Mida_Simulacio") == "Molt_Gran") {
            lSizeRiver = 2.7;
        }
        if(mValues.at("Conte_Riu") == "on")
            gBaseToRiver<int> gBTR(gB, 20, lSizeRiver, 100);

        std::shared_ptr<gLayerAirPollution> gLAP = std::make_shared<gLayerAirPollution>(gLayerAirPollution(gB));
        gLAP->setTransformation({0, 1, 2, 3, 4, 5});
        std::shared_ptr<gSimLayers> gSimL = std::make_shared<gSimLayers>(gSimLayers(gLAP));

        std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>();
        rGlobal rG(gSimL, pPM);
        rG.setUp();
        while (rG.isOpen) {
            rG.loop();
        }
        return 0;
    }


};

#endif //CITYOFWEIRDFISHES_SIMINITIALIZE_H
