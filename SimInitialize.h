//
// Created by Laminar on 17/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SIMINITIALIZE_H
#define CITYOFWEIRDFISHES_SIMINITIALIZE_H

#include "src/sim/structure/grids/transformation/gBaseToRiver.h"
#include "src/sim/structure/grids/transformation/gBaseToLineRoads.h"
#include "src/sim/structure/grids/transformation/gBaseToPattern.h"
#include "src/display/menus/implementations/rBaseMenu.h"

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

        std::shared_ptr<gIGrid<uint32_t>> gB = std::make_shared<gBasicGrid<uint32_t>>(
                gBasicGrid<uint32_t>(lSizeGrid, lSizeGrid, -1));

        std::vector<gtmElement> vElem = {
                gtmElement(0.0, 0, 0.0, 0),
                gtmElement(0.01, 0, 0.04, 0),
                gtmElement(0.07, 0, 0.1, 0),};


        std::shared_ptr<gIGrid<bool>> gUrbanCenterMask = std::make_shared<gBasicGrid<bool>>(
                gBasicGrid<bool>(lSizeGrid, lSizeGrid, true));


        std::vector<std::pair<int, int>> vPosCentre;
        if (mValues.at("Ciutat_Central") == "on")
            vPosCentre = {{lSizeGrid / 2,                 lSizeGrid / 2},
                          {lSizeGrid / 7,                 lSizeGrid / 3},
                          {lSizeGrid / 7 + lSizeGrid / 2, lSizeGrid / 4 + lSizeGrid / 2}};
        else
            vPosCentre = {{30, 30},
                          {70, 70}};
        for (int i = 0; i < std::stoi(mValues.at("Centres_Urbans")); i++) {
            gBaseToGradientMinimum gBGM(vElem, vPosCentre[i], gB, gUrbanCenterMask);
            gBGM.generateV2();
            gUrbanCenterMask = BasicTransformations::genMaskFromGrid(gB, {0});
            gB = BasicTransformations::replaceValues(gB, {{0, -1}});
        }
        gB = BasicTransformations::replaceValues(gB, {{-1, 0}});


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

        std::shared_ptr<gIGrid<bool>> gUrbanRoadsMask = std::make_shared<gBasicGrid<bool>>(
                gBasicGrid<bool>(lSizeGrid, lSizeGrid, true));
        gUrbanCenterMask = BasicTransformations::genMaskFromGrid(gB, {1, 2});
        //No pots ser simplement una mascara, ha de ser com el voltant de la ciutat i ha de tenir totes les caselles complertes per no deixar carreteres sense ser
        //Compertes.

        //RIVER
        if (mValues.at("Conte_Riu") == "on")
            gBaseToRiver<uint32_t> gBTR(gB, 20, lSizeRiver, 100);



        //RIVER TRANSFORMATION
        std::map<uint32_t, std::vector<std::pair<std::pair<int, int>, uint8_t>>> pRiver = gBaseToBorderDetection::generate<uint32_t>(
                gB, {gBorderType::gBNonConnex, gBorderOutside::gIsGroup}, {},
                BasicTransformations::genMaskFromGrid(gB, {5}));

        for (const auto &map_element: pRiver) {
            for (const auto &elm: map_element.second) {
                //8 code to 4:
                uint8_t p = 0;

                if ((elm.second & (1 << 6)) && (elm.second & (1 << 1))) {
                    p = (elm.second & (1 << 4) && !(elm.second & (1 << 3))) ? 1 : (elm.second & (1 << 3) &&
                                                                                   !(elm.second & (1 << 4))) ? 2 : 0;
                } else if ((elm.second & (1 << 3)) && (elm.second & (1 << 4))) {
                    p = (elm.second & (1 << 1) && !(elm.second & (1 << 6))) ? 3 : (elm.second & (1 << 6) &&
                                                                                   !(elm.second & (1 << 1))) ? 4 : 0;
                } else if ((elm.second & (1 << 4)) && (elm.second & (1 << 1))) {
                    p = 5;
                } else if ((elm.second & (1 << 3)) && (elm.second & (1 << 1))) {
                    p = 6;
                } else if ((elm.second & (1 << 3)) && (elm.second & (1 << 6))) {
                    p = 7;
                } else if ((elm.second & (1 << 6)) && (elm.second & (1 << 4))) {
                    p = 8;
                }

                gB->set({elm.first.second, elm.first.first},
                        (((uint32_t) (uint8_t) strtol("110000", NULL, 2)) << 24) + p);
            }
        }
        //END RIVER TRANSFORMATION


        if (mValues.at("Estructura_Ciutat") == "Graella") {
            gBaseToPattern gBP(gB,
                               gBaseToPattern<uint32_t>::gPatternType::gBPBlobSquares,
                               gBaseToPattern<uint32_t>::gPatternParameters(4, 4, 20, 20), gUrbanCenterMask);

        } else if (mValues.at("Estructura_Ciutat") == "Radial") {
            gBaseToPattern gBP(gB,
                               gBaseToPattern<uint32_t>::gPatternType::gBPSquares,
                               gBaseToPattern<uint32_t>::gPatternParameters(4, 4, 20, 20), gUrbanCenterMask);
        }

        //srand(static_cast<unsigned int>(time(0))); dodo change this
        for (int i = 0; i < std::stoi(mValues.at("Quanitat_Carrers_Princiapls")); i++)
            gBaseToLineRoads::givenFunction<uint32_t>(gB,
                                                      static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 6) - 3,
                                                      rand() % (lSizeGrid + 1));


        //TODO MILLORAR EL ORDRE EN EL QUE ES FA PER ARA ES FA DOS COPS:

        std::map<uint32_t, std::vector<std::pair<std::pair<int, int>, uint8_t>>> p =
                gBaseToBorderDetection::generate(gB, {gBorderType::gBNonConnex, gBorderOutside::gIsNotGroup}, {3, 4},
                                                 BasicTransformations::genMaskFromGrid(gB, {3,4}));

        for (const auto &map_element: p) {
            for (const auto &elm: map_element.second) {
                //8 code to 4:
                uint8_t p = ((elm.second & (1 << 1)) != 0) << 3
                            | ((elm.second & (1 << 4)) != 0) << 2
                            | ((elm.second & (1 << 6)) != 0) << 1
                            | ((elm.second & (1 << 3)) != 0);
                if(gB->get(elm.first.second, elm.first.first) == 3) {
                    gB->set({elm.first.second, elm.first.first},
                            (((uint32_t) (uint8_t) strtol("100001", NULL, 2)) << 24) + p);
                }else{
                    gB->set({elm.first.second, elm.first.first},
                            (((uint32_t) (uint8_t) strtol("100000", NULL, 2)) << 24) + p);
                }
            }
        }




        //std::shared_ptr<gLayerAirPollution> gLAP = std::make_shared<gLayerAirPollution>(gLayerAirPollution(gB));
        //gLAP->setTransformation({0, 1, 2, 3, 4, 5});


        gB = BasicTransformations::replaceValues(gB, {{2, ((uint32_t) (uint8_t) strtol("00010000", NULL, 2)) << 24}});
        gB = BasicTransformations::replaceValues(gB, {{1, ((uint32_t) (uint8_t) strtol("00010001", NULL, 2)) << 24}});

        std::shared_ptr<gLayerCity> gLC = std::make_shared<gLayerCity>(gLayerCity(gB));
        std::shared_ptr<gSimLayers> gSimL = std::make_shared<gSimLayers>(nullptr, gLC, gB->rangeUse());
        //gSimL->switchActual(gSimLayersTypes::G_AIRPOLLUTION);

        std::shared_ptr<rPileMenus> pPM = std::make_shared<rPileMenus>(gSimL);
        std::shared_ptr<rBaseMenu> rBasic = std::make_shared<rBaseMenu>(
                pPM, rIMenu::rRelativePos::pBottomRight);
        pPM->addMenuTop(rBasic);

        rGlobal rG(gSimL, pPM);
        rG.setUp();
        while (rG.isOpen) {
            rG.loop();
        }
        return 0;
    }


};

#endif //CITYOFWEIRDFISHES_SIMINITIALIZE_H
