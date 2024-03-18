//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_STOTALCOMPANY_H
#define CITYOFWEIRDFISHES_STOTALCOMPANY_H

#include <vector>
#include <memory>
#include "sCommon.h"
#include "../grids/gIGrid.h"
#include "../../behaviour/company/sCompanyCompiler.h"

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

    uint32_t addComp(const std::vector<std::pair<int, int>> &sTilesStart) {
        uint32_t prevFEmpty = fEmpty;
        fEmpty = tVecComp[fEmpty].first;
        tVecComp[prevFEmpty] = {0, std::make_shared<objCompany>(prevFEmpty, sTilesStart)};
        return prevFEmpty;
    }

    void removeComp(uint32_t rCar) {
        tVecComp[rCar] = {fEmpty, {}};
        fEmpty = rCar;
    }

    std::vector<sCompanyCompiler::sCCIntentions> getTotalIntentions() {
        std::vector<sCompanyCompiler::sCCIntentions> ret;
        for (auto &i: tVecComp) {
            if (i.second != nullptr) {
                std::vector<sCompanyCompiler::sCCIntentions> rComp =
                        sCompanyCompiler::givenCode(i.second->c_cCode, i.second);
                ret.insert(ret.end(), rComp.begin(), rComp.end());
            }
        }
        return ret;
    }

private:
    std::vector<std::pair<uint32_t, std::shared_ptr<objCompany>>> tVecComp;
    uint32_t fEmpty;
};

class sTotalCompany {
public:
    explicit sTotalCompany(uint32_t maxComp) : vTotalComp(maxComp) {}

    void addCompanyAtPosition(const std::shared_ptr<gIGrid<std::list<uint32_t>>> &gLayer,
                              const std::vector<std::pair<int, int>> &vecNPos) {
        uint32_t idNewComp = vTotalComp.addComp(vecNPos);
        for (const auto &nPos: vecNPos) {
            auto p = gLayer->get({nPos.second, nPos.first});
            p.push_front(idNewComp); //AAAAAAAAAAAAAAAAAAAAAA
            gLayer->set({nPos.second, nPos.first}, p);//TODO no m'agrada que aixo vulgi dir que estem creant una copia.
        }
    }

    std::shared_ptr<objCompany> getCompanyByUUID(uint32_t index) {
        return vTotalComp.getDestByComp(index);
    }

    std::vector<objCompany> getVectCompByUUID(const std::list<uint32_t> &tList) {
        std::vector<objCompany> r;
        for (const uint32_t l: tList) {
            r.push_back(*getCompanyByUUID(l));
        }
        return r;
    }

    std::vector<sCompanyCompiler::sCCIntentions> getTotalIntentions() { return vTotalComp.getTotalIntentions(); }

private:
    rVectorCompanies vTotalComp;
};


#endif //CITYOFWEIRDFISHES_STOTALCOMPANY_H
