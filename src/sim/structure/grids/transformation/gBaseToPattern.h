//
// Created by Laminar on 28/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETOPATTERN_H
#define CITYOFWEIRDFISHES_GBASETOPATTERN_H

#include <memory>
#include <vector>
#include <random>
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
                break;
            case gBPBlobSquares:
                break;
        }

        gen.seed(seed);
    }

private:

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
        if(0 <= dOffsets[dir].first && dOffsets[dir].first < gSquares.size() && 0 <= dOffsets[dir].second && dOffsets[dir].second < gSquares[0].size()){
            if(gSquares[dOffsets[dir].first][dOffsets[dir].second] > gSquares[i][j]){
                gSquares[dOffsets[dir].first][dOffsets[dir].second] = gSquares[i][j];
            }else{
                gSquares[i][j] = gSquares[dOffsets[dir].first][dOffsets[dir].second];
            }
        }

        return gSquares;
    }

    std::shared_ptr<gIGrid<T>> gtmGResult;

};

#endif //CITYOFWEIRDFISHES_GBASETOPATTERN_H
