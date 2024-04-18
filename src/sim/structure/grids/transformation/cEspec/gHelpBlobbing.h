//
// Created by Laminar on 28/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GHELPBLOBBING_H
#define CITYOFWEIRDFISHES_GHELPBLOBBING_H

#include <memory>
#include <vector>
#include <list>
#include <random>
#include "../../gIGrid.h"
#include "../../gBasicGrid.h"

class gHelpBlobbing {

public:
    static std::vector<std::vector<int>> genGridUnique(int height, int width) {
        std::vector<std::vector<int>> grid(height, std::vector<int>(width));
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                grid[i][j] = i * width + j;
            }
        }
        return grid;
    }

    static std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>>
    genListOfUniquePos(std::vector<std::vector<int>> gSquares) {
        if(gSquares.empty())
            return {};
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

    static std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>>
    blobTwoPositions(std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>> gSquares,
                     std::mt19937& gen,
                     std::uniform_int_distribution<>& dis_row,
                     std::uniform_int_distribution<>& dis_col,
                     std::uniform_int_distribution<>& dis_dir) {
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

    //BLOBY GRID V2
    template<typename T>
    static std::shared_ptr<gIGrid<T>>
    blobToGrid(std::vector<std::vector<std::shared_ptr<std::list<std::pair<int, int>>>>> gInput) {
        std::shared_ptr<gIGrid<T>> gB =
                std::make_shared<gBasicGrid<T>>(gBasicGrid<T>(gInput.size(), gInput[0].size(), -1));
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

};

#endif //CITYOFWEIRDFISHES_GHELPBLOBBING_H
