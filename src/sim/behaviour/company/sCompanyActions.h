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

class sCompanyActions {

    static bool gProduceProduct(const objCompany &oC, uint32_t gItemGen, sTotalRecipes &sTR,
                                const std::shared_ptr<gIGrid<uint8_t>> &gType) {
        if (!hasResources(oC, gItemGen, sTR) || !hasTypeOwn(oC, gItemGen, sTR, gType))
            return false;

        //TODO..
        //Restar els elements del map de la empresa;
        //Posar un timer amb el temps necessari que sera comprobat cada tick per si es aquell
        //Afegir li la id +1 en el final.

        return true;
    }


private:

    static bool hasTypeOwn(const objCompany &oC, uint32_t gItemGen, sTotalRecipes &sTR,
                           const std::shared_ptr<gIGrid<uint8_t>> &gType) {
        std::unordered_set<uint8_t> companyOwnTypes;

        for (const auto &p: oC.c_cOwn)
            companyOwnTypes.insert(gType->get(p));

        for (const auto &n: sTR.getById(gItemGen).pr_reqBuilding)
            if (companyOwnTypes.find(n) != companyOwnTypes.end())
                return true;

        return false;
    }

    static bool hasResources(const objCompany &oC, uint32_t gItemGen, sTotalRecipes &sTR) {
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
