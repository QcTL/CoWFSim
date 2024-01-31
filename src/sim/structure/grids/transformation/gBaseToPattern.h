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


template<typename T>
class gBaseToPattern {
public:
    enum gPatternType {
        gBPSquares, gBPBlobSquares
    };

    gBaseToPattern(std::shared_ptr<gIGrid<T>> gR, gPatternType type, int seed) : gtmGResult(gR) {
        switch (type) {
            case gBPSquares:
                genBasicSquaresToGrid(7, 7, 7, 7);
                genDiagonalToGrid(35);
                break;
            case gBPBlobSquares:
                gen.seed(seed);
                break;
        }
    }

private:

    void genBasicSquaresToGrid(int bWidth, int bHeight, int nHeight, int nWidth) {
        std::pair<std::pair<int, int>, std::pair<int, int>> ranges = gtmGResult->rangeUse();
        for (int x = 0; x < nWidth; x++) {
            for (int y = 0; y < nHeight; y++) {
                std::pair<int, int> pTL = {x * (bWidth - 1), y * (bHeight - 1)};
                std::cout << pTL.first << " - " << pTL.second << std::endl;

                for (int i = 0; i < bHeight; i++) {
                    gtmGResult->set(pTL.first, pTL.second + i, 1);
                    gtmGResult->set(pTL.first + bWidth - 1, pTL.second + i, 1);
                }

                for (int i = 0; i < bWidth; i++) {
                    gtmGResult->set(pTL.first + i, pTL.second, 1);
                    gtmGResult->set(pTL.first + i, pTL.second + bHeight - 1, 1);
                }

                //gtmGResult->set(pTL.first, pTL.second, 2);
            }
        }
    }

    void genDiagonalToGrid(int lenghtDiag) {
        for (int i = 0; i < lenghtDiag; i++) {
            gtmGResult->set(i, i, 1);
            gtmGResult->set(i, i - 1, 1);
        }
    }

    std::vector<std::vector<int>> genGridSquares(int height, int width) {
        dis_row = std::uniform_int_distribution<>(0, height);
        dis_col = std::uniform_int_distribution<>(0, width);
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

    std::mt19937 gen;
    std::uniform_int_distribution<> dis_row;
    std::uniform_int_distribution<> dis_col;
    std::uniform_int_distribution<> dis_dir;


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


    std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>>
    prepBlobSquares(std::vector<std::vector<int>> gSquares) {
        std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>> ret(gSquares.size(),
                                                                                      std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>(
                                                                                              gSquares[0].size()));
        for (int i = 0; i < gSquares.size(); i++) {
            for (int j = 0; j < gSquares.size(); j++) {
                ret[i][j]->emplace_back(i, j);
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

        gSquares[i][j]->insert(gSquares[i][j]->end(),
                               gSquares[dOffsets[dir].first][dOffsets[dir].second]->begin(),
                               gSquares[dOffsets[dir].first][dOffsets[dir].second]->end());
        gSquares[dOffsets[dir].first][dOffsets[dir].second] = gSquares[i][j];

        return gSquares;
    }

    std::shared_ptr<gIGrid<T>> gtmGResult;

};

#endif //CITYOFWEIRDFISHES_GBASETOPATTERN_H
