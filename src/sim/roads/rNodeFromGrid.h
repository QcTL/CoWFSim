//
// Created by Laminar on 04/02/2024.
//

#ifndef CITYOFWEIRDFISHES_RNODEFROMGRID_H
#define CITYOFWEIRDFISHES_RNODEFROMGRID_H

#include <memory>
#include <vector>
#include <unordered_set>
#include "../structure/grids/gIGrid.h"
#include "node/rNode.h"

template<typename T>
class rNodeFromGrid {
public:

    static std::vector<rNode*> givenGrid(std::shared_ptr<gIGrid<T>> g, T vRoads) {
        std::vector<rNode*> ret;
        std::unordered_set<int> rIndexAlready;
        std::pair<std::pair<int, int>, std::pair<int, int>> gRange = g->rangeUse();
        int gWidth = (gRange.second.second - gRange.second.first) + 1;
        int gHeight = (gRange.first.second - gRange.first.first) + 1;
        std::vector<std::vector<rNode*>> mPointers(gWidth, std::vector<rNode*>(gHeight, nullptr));


        for (int i = gRange.first.first; i <= gRange.first.second; i++) {
            for (int j = gRange.second.first; j <= gRange.second.second; j++) {
                if (g->get(i, j) == vRoads && (rIndexAlready.find(j + gWidth*i) == rIndexAlready.end())) {
                    auto* n = new rNode({i,j});
                    mPointers[i][j] = n;
                    std::cout << "[" << i << "]"<< "[" << j << "]" << std::endl;
                    discoverRec(n, g, rIndexAlready, vRoads, {i,j}, gWidth, mPointers);
                    ret.push_back(n);
                }
            }
        }
        return ret;
    }

private:
    static void
    discoverRec(rNode* n, std::shared_ptr<gIGrid<T>> g, std::unordered_set<int> &rIndexAlready, T vRoads, std::pair<int, int> p, int& gWidth, std::vector<std::vector<rNode*>>& mPointers) {
        std::vector<std::pair<int, int>> dOffsets =
                {{p.first,     p.second + 1},
                 {p.first - 1, p.second},
                 {p.first + 1, p.second},
                 {p.first,     p.second - 1}};
        rIndexAlready.insert(p.second + gWidth*p.first);
        for (int i =0; i < dOffsets.size(); i++) {
            std::pair<int, int> nP = dOffsets[i];
            if (g->isInside(nP.first, nP.second) && g->get(nP.first, nP.second) == vRoads && (rIndexAlready.find(nP.second + gWidth*nP.first) == rIndexAlready.end())) {
                rNode* nNew = mPointers[nP.first][nP.second];
                std::cout << "[" << nP.first << "]"<< "[" << nP.second << "]" << std::endl;
                if(nNew == nullptr){
                    nNew = new rNode({nP.first,nP.second});
                    mPointers[nP.first][nP.second] = nNew;
                }
                if(i == 0){
                    n->rBottom = nNew;
                    nNew->rTop = n;
                }else if(i == 1){
                    n->rLeft = nNew;
                    nNew->rRight = n;
                }else if(i == 2){
                    n->rRight = nNew;
                    nNew->rLeft = n;
                }else{
                    n->rTop = nNew;
                    nNew->rBottom = n;
                }
                //discoverRec(*nNew, g, rIndexAlready, vRoads, nP, gWidth);
            }
        }
        for (int i =0; i < dOffsets.size(); i++) {
            std::pair<int, int> nP = dOffsets[i];
            if (g->isInside(nP.first, nP.second) && g->get(nP.first, nP.second) == vRoads && (rIndexAlready.find(nP.second + gWidth*nP.first) == rIndexAlready.end())) {
                if (i == 0) { discoverRec(n->rBottom, g, rIndexAlready, vRoads, nP, gWidth, mPointers);
                } else if (i == 1) { discoverRec(n->rLeft, g, rIndexAlready, vRoads, nP, gWidth, mPointers);
                } else if (i == 2) { discoverRec(n->rRight, g, rIndexAlready, vRoads, nP, gWidth, mPointers);
                } else { discoverRec(n->rTop, g, rIndexAlready, vRoads, nP, gWidth, mPointers); }
            }
        }
    }


};

#endif //CITYOFWEIRDFISHES_RNODEFROMGRID_H
