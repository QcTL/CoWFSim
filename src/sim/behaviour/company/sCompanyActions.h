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
#include "../market/sMarketListing.h"

class sCompanyActions {
public:

    sCompanyActions(sTotalRecipes &sTR,
                    const std::shared_ptr<gIGrid<uint8_t>> &gType,
                    const std::shared_ptr<sCompanyTimer> &gCTimer)
            : sCA_CTR(sTR), sCA_gTimer(gCTimer), sCA_gType(gType) {}

    bool gTryIntention(sCompanyCompiler::sCCIntentions &sCCI,
                       const std::shared_ptr<gIGrid<uint8_t>> &gAirPollution,
                       const std::shared_ptr<gIGrid<uint8_t>> &gTypeSoil, uint32_t actTimer) {
        switch (sCCI.scc_type) {
            case sCompanyCompiler::sCCIntentions::CELL_Buy: {
                sMarketListing::sMOffering sMOff = sCA_MarketListing->getOfferingByType(sCCI.scc_addIdInfo, sCA_gType);
                //TODO some evaluation and add the element; and remove it from the owner;
                sCA_MarketListing->buyOffering(sMOff);
            }
                break;
            case sCompanyCompiler::sCCIntentions::CELL_GiveRent:
                break;
            case sCompanyCompiler::sCCIntentions::CELL_GainRent:
                break;
            case sCompanyCompiler::sCCIntentions::CELL_Sell:
                sCA_MarketListing->addListing(
                        {sCCI.scc_objCompany->c_cOwn[sCCI.scc_addIdInfo % sCCI.scc_objCompany->c_cOwn.size()],
                         0, 0});
                break;
            case sCompanyCompiler::sCCIntentions::OBJ_Produce:
                gProduceProduct(sCCI.scc_objCompany, sCCI.scc_addIdInfo, actTimer);
                break;
        }
        return true;
    }

    static void gCompletedProduct(const std::shared_ptr<objCompany> &oC, uint32_t gItemGen) {
        if (oC->c_pOwn.find(gItemGen) == oC->c_pOwn.end())
            oC->c_pOwn[gItemGen] = 1;
        else
            oC->c_pOwn[gItemGen] += 1;
    }

private:

    bool gProduceProduct(std::shared_ptr<objCompany> &oC, uint32_t gItemGen, uint32_t actTime) {
        if (!hasResources(*oC, gItemGen, sCA_CTR) || !hasTypeOwn(*oC, gItemGen, sCA_CTR, sCA_gType))
            return false;

        for (const auto &gElem: sCA_CTR.getById(gItemGen).pr_reqProdId)
            oC->c_pOwn[gElem] -= 1;

        sCA_gTimer->addTimer(gItemGen, sCA_CTR.getById(gItemGen).pr_reqTime + actTime, oC->c_uuid);
        return true;
    }

    //ELEMENTS VALIDATIONS;
    bool hasTypeOwn(objCompany &oC, uint32_t gItemGen, sTotalRecipes &sTR,
                    const std::shared_ptr<gIGrid<uint8_t>> &gType) {
        std::unordered_set<uint8_t> companyOwnTypes;

        for (const auto &p: oC.c_cOwn)
            companyOwnTypes.insert(gType->get(p));

        for (const auto &n: sTR.getById(gItemGen).pr_reqBuilding)
            if (companyOwnTypes.find(n) != companyOwnTypes.end())
                return true;

        return false;
    }

    bool hasResources(objCompany &oC, uint32_t gItemGen, sTotalRecipes &sTR) {
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

    sTotalRecipes sCA_CTR;
    std::shared_ptr<gIGrid<uint8_t>> sCA_gType;
    std::shared_ptr<sCompanyTimer> sCA_gTimer;
    std::shared_ptr<sMarketListing> sCA_MarketListing;
};

#endif //CITYOFWEIRDFISHES_SCOMPANYACTIONS_H
