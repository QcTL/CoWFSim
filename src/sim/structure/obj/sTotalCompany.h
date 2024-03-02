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
        tVecComp = std::vector<std::pair<uint32_t, std::shared_ptr<objCompany>>>(nMaxCompanies, {-1, nullptr});
        for (int i = 0; i < nMaxCompanies - 1; i++) {
            tVecComp[i] = {i + 1, nullptr};
        }
        tVecComp[tVecComp.size() - 1] = {-1, nullptr};
        fEmpty = 0;
    }

    std::shared_ptr<objCompany> getDestByComp(uint32_t rCar) {
        return tVecComp[rCar].second;
    }

    uint32_t addComp(uint32_t destPos) {
        uint32_t prevFEmpty = fEmpty;
        fEmpty = tVecComp[fEmpty].first;
        tVecComp[prevFEmpty] = {destPos, std::make_shared<objCompany>(prevFEmpty)};
        return prevFEmpty;
    }

    void removeComp(uint32_t rCar) {
        tVecComp[rCar] = {fEmpty, {}};
        fEmpty = rCar;
    }

private:
    std::vector<std::pair<uint32_t, std::shared_ptr<objCompany>>> tVecComp;
    uint32_t fEmpty;
};

class sTotalCompany {
public:
    explicit sTotalCompany(uint32_t maxComp) : vTotalComp(maxComp) {}

    void addCompanyAtPosition(const std::shared_ptr<gIGrid<std::list<uint32_t>>> &gLayer,
                              const std::pair<uint32_t, uint32_t> nPos) {
        auto p = gLayer->get(nPos);
        p.push_front(vTotalComp.addComp(0));
        gLayer->set(nPos, p);
    }

    std::shared_ptr<objCompany> getCompanyByUUID(uint32_t index){
        return vTotalComp.getDestByComp(index);
    }

    std::vector<objCompany> getVectCompByUUID(const std::list<uint32_t>& tList){
        std::vector<objCompany> r;
        for(const uint32_t l : tList){
            r.push_back(*getCompanyByUUID(l));
        }
        return r;
    }

private:
    rVectorCompanies vTotalComp;
};


#endif //CITYOFWEIRDFISHES_STOTALCOMPANY_H
