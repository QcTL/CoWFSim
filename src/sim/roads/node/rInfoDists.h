//
// Created by Laminar on 11/02/2024.
//

#ifndef CITYOFWEIRDFISHES_RINFODISTS_H
#define CITYOFWEIRDFISHES_RINFODISTS_H

#include <bitset>
#include <iostream>
#include "RNodeAttrs/rRMailbox.h"

class rInfoDist{
public:
    static bool addIfShorter(const rRMail &r, const uint32_t &GIdNode,const uint16_t &actGrid, const uint8_t &dir){

        uint32_t indexPos =
                (r.mGridOrigin == actGrid) ? uTotalBlocks+ r.mRoadStart: (r.mGridOrigin + uTotalBlocks - actGrid) % uTotalBlocks;

        //Caution no es simplement mRoadEnd, es allo de primer blocs i despres la mRoadEnd
        if((dMatrixDist[GIdNode][indexPos]& ~(0x7u << 29)) > (r.mSizePath & ~(0x7u << 29)) || !(dMatrixDist[GIdNode][indexPos] & (1u << 31))){
            //Deixem el 30 i 29 bit en la dir correcte i el primer bit esquerra com a 1 per ja estar assignat
            dMatrixDist[GIdNode][indexPos] = ((dMatrixDist[GIdNode][indexPos] & ~(0x3u << 29)) | ((dir & 0x3) << 29)) | (1u << 31);
            //Afegim la distancia
            //std::bitset<32> binary(dMatrixDist[GIdNode][indexPos]);
            //std::cout <<binary  << std::endl;

            dMatrixDist[GIdNode][indexPos] =  dMatrixDist[GIdNode][indexPos] & (0x7u << 29) | r.mSizePath & ~(0x7u << 29);
            //std::bitset<32> binary2(dMatrixDist[GIdNode][indexPos]);
            //std::cout << binary2  << std::endl;
            return true;
        }
        return false;
    }

    static uint8_t returnDirToDist(const uint32_t destNode, const uint16_t destGrid, const uint16_t actGrid, const uint32_t uidNodeAct){
        uint32_t indexPos =
                (destGrid == actGrid) ? uTotalBlocks + destNode: (destGrid + uTotalBlocks - actGrid) % uTotalBlocks;
        return dMatrixDist[uidNodeAct][indexPos] != 0 ? (dMatrixDist[uidNodeAct][indexPos] >> 29) & 3 : 4;
    }

    static void initializeMatrix(uint32_t uNumBlocks, uint32_t uNumMaxRoadForBlock, uint32_t nTotalRoads){
        dMatrixDist = std::vector<std::vector<uint32_t>>(nTotalRoads, std::vector<uint32_t>(uNumBlocks + uNumMaxRoadForBlock, 0));
        uTotalBlocks = uNumBlocks;
    }

    static void seeMatrix(){
        std::cout<< "this is a latch to see the matrix" << std::endl;
    }

    static void addSelfDist(uint32_t uidNode) {
        uint32_t indexPos = uTotalBlocks + uidNode;
        dMatrixDist[uidNode][indexPos] = (dMatrixDist[uidNode][indexPos] & ~(0x3u << 29)) | (1u << 31);
    }

private:
    static std::vector<std::vector<uint32_t>> dMatrixDist;
    static uint32_t uTotalBlocks;
};
std::vector<std::vector<uint32_t>> rInfoDist::dMatrixDist;
uint32_t rInfoDist::uTotalBlocks;

#endif //CITYOFWEIRDFISHES_RINFODISTS_H
