//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_STOTALCOMPANY_H
#define CITYOFWEIRDFISHES_STOTALCOMPANY_H

#include <vector>
#include <memory>
#include "sCommon.h"
#include "../grids/gIGrid.h"

class rVectorCompanies {
public:

    rVectorCompanies(uint32_t nMaxCompanies) {
        tVecComp = std::vector<std::pair<uint32_t, std::shared_ptr<obj_company>>>(nMaxCompanies, {-1, nullptr});
        for (int i = 0; i < nMaxCompanies - 1; i++) {
            tVecComp[i] = {i + 1, nullptr};
        }
        tVecComp[tVecComp.size() - 1] = {-1, nullptr};
        fEmpty = 0;
    }

    std::shared_ptr<obj_company> getDestByComp(uint32_t rCar) {
        return tVecComp[rCar].second;
    }

    uint32_t addComp(uint32_t destPos) {
        uint32_t prevFEmpty = fEmpty;
        fEmpty = tVecComp[fEmpty].first;
        tVecComp[prevFEmpty] = {destPos, std::make_shared<obj_company>(prevFEmpty)};
        return prevFEmpty;
    }

    void removeComp(uint32_t rCar) {
        tVecComp[rCar] = {fEmpty, {}};
        fEmpty = rCar;
    }

private:
    std::vector<std::pair<uint32_t, std::shared_ptr<obj_company>>> tVecComp;
    uint32_t fEmpty;
};

class sTotalCompany {
public:
    explicit sTotalCompany(uint32_t maxComp) : vTotalComp(maxComp) {}

    void addCompanyAtPosition(const std::shared_ptr<gIGrid<std::list<uint32_t>>> &gLayer,
                              const std::pair<uint32_t, uint32_t> nPos) {
        gLayer->get(nPos).push_front(vTotalComp.addComp(0));
    }

    std::shared_ptr<obj_company> getCompanyByUUID(uint32_t index){
        return vTotalComp.getDestByComp(index);
    }

    std::list<obj_company> getListCompByUUID(const std::list<uint32_t>& tList){
        std::list<obj_company> r;
        for(const uint32_t l : tList){
            r.push_front(*getCompanyByUUID(l));
        }
        return r;
    }

private:
    rVectorCompanies vTotalComp;
};


#endif //CITYOFWEIRDFISHES_STOTALCOMPANY_H
