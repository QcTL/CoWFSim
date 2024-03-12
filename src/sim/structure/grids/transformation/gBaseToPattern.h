//
// Created by Laminar on 28/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETOPATTERN_H
#define CITYOFWEIRDFISHES_GBASETOPATTERN_H

#include <memory>
#include <vector>
#include <random>
#include <list>
#include "../gIGrid.h"
#include "../gBasicGrid.h"
#include "gBaseToBorderDetection.h"
#include "cEspec/gHelpBlobbing.h"


template<typename T>
class gBaseToPattern {
public:
    enum gPatternType {
        gBPSquares, gBPBlobSquares
    };

    struct gPatternParameters {
        int sizeBlockHeight;
        int sizeBlockWidth;
        int nBlocksX;
        int nBlocksY;

        gPatternParameters(int sizeBlockHeight, int sizeBlockWidth, int nBlocksX, int nBlocksY) : sizeBlockHeight(
                sizeBlockHeight), sizeBlockWidth(sizeBlockWidth), nBlocksX(nBlocksX), nBlocksY(nBlocksY) {}
    };

    gBaseToPattern(std::shared_ptr<gIGrid<T>> gR, gPatternType type, gPatternParameters parameters, T endValue,
                   const std::shared_ptr<gIGrid<bool>> &pMask = nullptr, int seed = -1)
            : gtmGResult(gR), hasMask(pMask != nullptr), gMask(pMask), gtmEndValue(endValue) {
        switch (type) {
            case gBPSquares:
                genBasicSquaresToGrid(parameters.sizeBlockWidth, parameters.sizeBlockHeight, parameters.nBlocksY,
                                      parameters.nBlocksX);
                //genDiagonalToGrid(35);
                break;
            case gBPBlobSquares:
                gen.seed(seed);

                std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>> g =
                        gHelpBlobbing::genListOfUniquePos(
                                gHelpBlobbing::genGridUnique(parameters.nBlocksY, parameters.nBlocksX));

                dis_row = std::uniform_int_distribution<>(0, parameters.nBlocksX - 1);
                dis_col = std::uniform_int_distribution<>(0, parameters.nBlocksY - 1);
                dis_dir = std::uniform_int_distribution<>(0, 3);

                for (int i = 0; i < 30; i++) {
                    g = gHelpBlobbing::blobTwoPositions(g, gen, dis_row, dis_col, dis_dir);
                }

                std::shared_ptr<gIGrid<T>> gGrid = gHelpBlobbing::blobToGrid<T>(g);
                auto p = gBaseToBorderDetection::generate(gGrid,
                                                          {gBorderType::gBNonConnex, gBorderOutside::gIsNotGroup}, {});
                blobRoadsToGrid(p, gR, parameters.sizeBlockWidth, parameters.sizeBlockHeight);
                break;
        }
    }

private:
    //MASK:
    bool hasMask;
    std::shared_ptr<gIGrid<bool>> gMask;

    void genBasicSquaresToGrid(int bWidth, int bHeight, int nHeight, int nWidth) {
        std::pair<std::pair<int, int>, std::pair<int, int>> ranges = gtmGResult->rangeUse();
        for (int y = 0; y < nHeight; y++) {
            for (int x = 0; x < nWidth; x++) {
                std::pair<int, int> pTL = {x * (bWidth - 1), y * (bHeight - 1)};

                if (gtmGResult->isInside(x, y) && (!hasMask || gMask->get(x, y))) {
                    for (int i = 0; i < bHeight; i++) {
                        gtmGResult->set(pTL.first, pTL.second + i, gtmEndValue);
                        gtmGResult->set(pTL.first + bWidth - 1, pTL.second + i, gtmEndValue);
                    }

                    for (int i = 0; i < bWidth; i++) {
                        gtmGResult->set(pTL.first + i, pTL.second, 1);
                        gtmGResult->set(pTL.first + i, pTL.second + bHeight - 1, gtmEndValue);
                    }
                }
                //gtmGResult->set(pTL.first, pTL.second, 2);
            }
        }
    }

    std::mt19937 gen;
    std::uniform_int_distribution<> dis_row;
    std::uniform_int_distribution<> dis_col;
    std::uniform_int_distribution<> dis_dir;

    void blobRoadsToGrid(std::map<T, std::vector<std::pair<std::pair<int, int>, uint8_t>>> edges,
                         std::shared_ptr<gIGrid<T>> gFinal, int sSquareWidth, int sSquareHeight) {
        for (const auto &mapElem: edges) {
            for (const auto &vecElem: mapElem.second) {
                std::pair<int, int> pair = vecElem.first;
                pair = {pair.first * (sSquareWidth - 1), pair.second * (sSquareHeight - 1)};
                if (gtmGResult->isInside(pair) && (!hasMask || gMask->get(pair))) {
                    bool HasBttm = ((vecElem.second & (1 << 1)) == 0);
                    bool HasTop = ((vecElem.second & (1 << 6)) == 0);
                    bool HasLeft = ((vecElem.second & (1 << 3)) == 0);
                    bool HasRight = ((vecElem.second & (1 << 4)) == 0);

                    for (int i = 0; i < sSquareWidth; i++) {
                        if (HasBttm) gtmGResult->set(pair.first + i, pair.second + sSquareHeight - 1, gtmEndValue);
                        if (HasTop) gtmGResult->set(pair.first + i, pair.second, gtmEndValue);
                    }

                    for (int i = 0; i < sSquareHeight; i++) {
                        if (HasLeft) gtmGResult->set(pair.first, pair.second + i, gtmEndValue);
                        if (HasRight) gtmGResult->set(pair.first + sSquareWidth - 1, pair.second + i, gtmEndValue);
                    }

                }
            }
        }
    }

    std::shared_ptr<gIGrid<T>> gtmGResult;
    T gtmEndValue;
};

#endif //CITYOFWEIRDFISHES_GBASETOPATTERN_H
