//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMPANYACTIONS_H
#define CITYOFWEIRDFISHES_SCOMPANYACTIONS_H

#include <set>
#include <unordered_set>
#include "../../structure/obj/sCommon.h"
#include "production/sTotalRecipes.h"
#include "../../structure/grids/gIGrid.h"
#include "sCompanyTimer.h"

class sCompanyActions {
public:
    static bool gProduceProduct(std::shared_ptr<objCompany> &oC, uint32_t gItemGen, sTotalRecipes &sTR,
                                const std::shared_ptr<gIGrid<uint8_t>> &gType,
                                const std::shared_ptr<sCompanyTimer> &gCTimer) {
        if (!hasResources(*oC, gItemGen, sTR) || !hasTypeOwn(*oC, gItemGen, sTR, gType))
            return false;

        for (const auto &gElem: sTR.getById(gItemGen).pr_reqProdId)
            oC->c_pOwn[gElem] -= 1;

        gCTimer->addTimer(gItemGen, sTR.getById(gItemGen).pr_reqTime, oC->c_uuid);
        return true;
    }

    static void gCompletedProduct(const std::shared_ptr<objCompany> &oC, uint32_t gItemGen) {
        if (oC->c_pOwn.find(gItemGen) != oC->c_pOwn.end())
            oC->c_pOwn[gItemGen] = 1;
        else
            oC->c_pOwn[gItemGen] += 1;
    }

private:

    static bool hasTypeOwn(objCompany &oC, uint32_t gItemGen, sTotalRecipes &sTR,
                           const std::shared_ptr<gIGrid<uint8_t>> &gType) {
        std::unordered_set<uint8_t> companyOwnTypes;

        for (const auto &p: oC.c_cOwn)
            companyOwnTypes.insert(gType->get(p));

        for (const auto &n: sTR.getById(gItemGen).pr_reqBuilding)
            if (companyOwnTypes.find(n) != companyOwnTypes.end())
                return true;

        return false;
    }

    static bool hasResources(objCompany &oC, uint32_t gItemGen, sTotalRecipes &sTR) {
        std::map<uint32_t, uint8_t> gQuant;
        objProdRecipe oPR = sTR.getById(gItemGen);
        for (const auto &nObj: oPR.pr_reqProdId)
            gQuant[nObj]++;

        for (const auto &pair: gQuant) {
            if (oC.c_pOwn.find(pair.first) == oC.c_pOwn.end() || oC.c_pOwn.at(pair.first) < pair.second)
                return false;
        }

        return true;
    }


};

#endif //CITYOFWEIRDFISHES_SCOMPANYACTIONS_H
