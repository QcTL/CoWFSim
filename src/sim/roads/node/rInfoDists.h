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
    static bool addIfShorter(const rRMail &r, const uint32_t &uidNode,const uint8_t &dir){
        //Caution no es simplement mRoadEnd, es allo de primer blocs i despres la mRoadEnd
        if((dMatrixDist[r.mRoadEnd][uidNode]& ~(0x7u << 29)) < (r.mSizePath & ~(0x7u << 29)) || !(dMatrixDist[r.mRoadEnd][uidNode] & (1u << 31))){
            //Deixem el 30 i 29 bit en la dir correcte i el primer bit esquerra com a 1 per ja estar assignat
            dMatrixDist[r.mRoadEnd][uidNode] = ((dMatrixDist[r.mRoadEnd][uidNode] & ~(0x3u << 29)) | ((dir & 0x3) << 29)) | (1u << 31);
            //Afegim la distancia
            std::bitset<32> binary(dMatrixDist[r.mRoadEnd][uidNode]);
            std::cout <<binary  << std::endl;

            dMatrixDist[r.mRoadEnd][uidNode] =  dMatrixDist[r.mRoadEnd][uidNode] & (0x7u << 29) | r.mSizePath & ~(0x7u << 29);
            std::bitset<32> binary2(dMatrixDist[r.mRoadEnd][uidNode]);
            std::cout << binary2  << std::endl;
            return true;
        }
        return false;
    }

    static void initializeMatrix(uint32_t uNumBlocks, uint32_t uNumMaxRoadForBlock){
        dMatrixDist = std::vector<std::vector<uint32_t>>(uNumBlocks, std::vector<uint32_t>(uNumMaxRoadForBlock, 0));
    }

private:
    static std::vector<std::vector<uint32_t>> dMatrixDist;
};
std::vector<std::vector<uint32_t>> rInfoDist::dMatrixDist;

#endif //CITYOFWEIRDFISHES_RINFODISTS_H
