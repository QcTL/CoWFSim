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

    gBaseToPattern(std::shared_ptr<gIGrid<T>> gR, gPatternType type, gPatternParameters parameters,
                   const std::shared_ptr<gIGrid<bool>> &pMask = nullptr ,int seed = -1)
            : gtmGResult(gR), hasMask(pMask != nullptr), gMask(pMask) {
        switch (type) {
            case gBPSquares:
                genBasicSquaresToGrid(parameters.sizeBlockWidth, parameters.sizeBlockHeight, parameters.nBlocksY,
                                      parameters.nBlocksX);
                //genDiagonalToGrid(35);
                break;
            case gBPBlobSquares:
                gen.seed(seed);

                std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>>
                        g = prepBlobSquares(genGridSquares(parameters.nBlocksY, parameters.nBlocksX));
                for (int i = 0; i < 30; i++) {
                    g = blobTwoSquaresV2(g);
                }
                std::shared_ptr<gIGrid<T>> gGrid = blobToGrid(g);

                gBaseToBorderDetection<T> gbDet(gGrid, gBorderType::gBNonConnex);
                std::map<T, std::vector<std::pair<std::pair<int, int>, uint8_t>>> p = gbDet.generate(
                        [](T i) { return true; });
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

                if(gtmGResult->isInside(x,y) && (!hasMask || gMask->get(x,y))) {
                    for (int i = 0; i < bHeight; i++) {
                        gtmGResult->set(pTL.first, pTL.second + i, 3);
                        gtmGResult->set(pTL.first + bWidth - 1, pTL.second + i, 3);
                    }

                    for (int i = 0; i < bWidth; i++) {
                        gtmGResult->set(pTL.first + i, pTL.second, 1);
                        gtmGResult->set(pTL.first + i, pTL.second + bHeight - 1, 3);
                    }
                }
                //gtmGResult->set(pTL.first, pTL.second, 2);
            }
        }
    }

    //BLOBY GRID V1
    std::vector<std::vector<int>> genGridSquares(int height, int width) {
        dis_row = std::uniform_int_distribution<>(0, height - 1);
        dis_col = std::uniform_int_distribution<>(0, width - 1);
        dis_dir = std::uniform_int_distribution<>(0, 3);

        std::vector<std::vector<int>> grid(height, std::vector<int>(width));
        //TA mal ha de ser un vector de punters no d'objectes
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                grid[i][j] = i * width + j;
            }
        }

        return grid;
    }

    std::vector<std::vector<int>> blobTwoSquares(std::vector<std::vector<int>> gSquares) {
        int i = dis_row(gen);
        int j = dis_col(gen);
        int dir = dis_dir(gen);

        std::vector<std::pair<int, int>> dOffsets =
                {{i + 1, j},
                 {i - 1, j},
                 {i,     j + 1},
                 {i,     j - 1}};

        //IF IS INSIDE
        if (0 <= dOffsets[dir].first && dOffsets[dir].first < gSquares.size() && 0 <= dOffsets[dir].second &&
            dOffsets[dir].second < gSquares[0].size()) {
            if (gSquares[dOffsets[dir].first][dOffsets[dir].second] > gSquares[i][j]) {
                gSquares[dOffsets[dir].first][dOffsets[dir].second] = gSquares[i][j];
            } else {
                gSquares[i][j] = gSquares[dOffsets[dir].first][dOffsets[dir].second];
            }
        }

        return gSquares;
    }

    std::mt19937 gen;
    std::uniform_int_distribution<> dis_row;
    std::uniform_int_distribution<> dis_col;
    std::uniform_int_distribution<> dis_dir;


    //BLOBY GRID V2
    std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>>
    prepBlobSquares(std::vector<std::vector<int>> gSquares) {
        std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>> ret(gSquares.size(),
                                                                                      std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>(
                                                                                              gSquares[0].size()));
        for (int i = 0; i < gSquares.size(); i++) {
            for (int j = 0; j < gSquares.size(); j++) {
                ret[i][j] = std::make_shared<std::list<std::pair<int, int>>>(
                        std::initializer_list<std::pair<int, int>>{{i, j}});
            }
        }
        return ret;
    }

    std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>>
    blobTwoSquaresV2(std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>> gSquares) {
        int i = dis_row(gen);
        int j = dis_col(gen);
        int dir = dis_dir(gen);

        std::vector<std::pair<int, int>> dOffsets =
                {{i + 1, j},
                 {i - 1, j},
                 {i,     j + 1},
                 {i,     j - 1}};
        if (0 <= dOffsets[dir].first && dOffsets[dir].first < gSquares.size() && 0 <= dOffsets[dir].second &&
            dOffsets[dir].second < gSquares[0].size()) {
            gSquares[i][j]->insert(gSquares[i][j]->end(),
                                   gSquares[dOffsets[dir].first][dOffsets[dir].second]->begin(),
                                   gSquares[dOffsets[dir].first][dOffsets[dir].second]->end());
            gSquares[dOffsets[dir].first][dOffsets[dir].second] = gSquares[i][j];
        }
        return gSquares;
    }

    std::shared_ptr<gIGrid<T>>
    blobToGrid(std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>> gInput) {
        std::shared_ptr<gIGrid<T>> gB = std::make_shared<gBasicGrid<T>>(
                gBasicGrid<T>(gInput.size(), gInput[0].size(), -1));

        for (int i = 0; i < gInput.size(); i++) {
            for (int j = 0; j < gInput[i].size(); j++) {
                for (auto &it: *gInput[i][j]) {
                    //TODO THIS OVERRIDES FOR EACH POSITION, WE HAVE TO KNOW IF WE ALLREADY DONE THAT LIST
                    //Maybe put the pointer to null if its has been done so when we find a null we dont have to check it more.
                    gB->set(it.first, it.second, i * gInput.size() + j);
                }
            }
        }

        return gB;
    }

    void blobRoadsToGrid(std::map<T, std::vector<std::pair<std::pair<int, int>, uint8_t>>> edges,
                         std::shared_ptr<gIGrid<T>> gFinal, int sSquareWidth, int sSquareHeight) {
        for (const auto &mapElem: edges) {
            for (const auto &vecElem: mapElem.second) {
                std::pair<int, int> pair = vecElem.first;
                pair = {pair.first * (sSquareWidth - 1), pair.second * (sSquareHeight - 1)};
                if(gtmGResult->isInside(pair) && (!hasMask || gMask->get(pair))) {
                    bool HasBttm = ((vecElem.second & (1 << 1)) == 0);
                    bool HasTop = ((vecElem.second & (1 << 6)) == 0);
                    bool HasLeft = ((vecElem.second & (1 << 3)) == 0);
                    bool HasRight = ((vecElem.second & (1 << 4)) == 0);

                    for (int i = 0; i < sSquareWidth; i++) {
                        if (HasBttm) gtmGResult->set(pair.first + i, pair.second + sSquareHeight - 1, 3);
                        if (HasTop) gtmGResult->set(pair.first + i, pair.second, 3);
                    }

                    for (int i = 0; i < sSquareHeight; i++) {
                        if (HasLeft) gtmGResult->set(pair.first, pair.second + i, 3);
                        if (HasRight) gtmGResult->set(pair.first + sSquareWidth - 1, pair.second + i, 3);
                    }

                }
            }
        }
    }


    std::shared_ptr<gIGrid<T>> gtmGResult;

};

#endif //CITYOFWEIRDFISHES_GBASETOPATTERN_H
