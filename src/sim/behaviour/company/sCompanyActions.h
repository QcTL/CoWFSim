//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMPANYACTIONS_H
#define CITYOFWEIRDFISHES_SCOMPANYACTIONS_H

#include <set>
#include <unordered_set>
#include "production/sTotalRecipes.h"
#include "../../structure/grids/gIGrid.h"
#include "sCompanyTimer.h"
#include "../market/sMarketBazaar.h"
#include "../gTerrainGrid.h"
#include "contract/sMainContractor.h"
#include "sCompanyCompiler.h"

class sCompanyActions {
public:
    sCompanyActions(sTotalRecipes &sTR,
                    const std::shared_ptr<gTerrainGrid> &gType,
                    const std::shared_ptr<sCompanyTimer> &gCTimer)
            : sCA_CTR(sTR), sCA_gTimer(gCTimer), sCA_gType(gType) {}

    bool
    gTryIntention(sCompanyCompiler::sCCIntentions &sCCI, std::shared_ptr<sCodeStorage> &sCodeStorage,
                  const uint32_t cDate) {
        switch (sCCI.scc_type) {
            case sCompanyCompiler::sCCIntentions::CELL_Buy: {
                sLBuyCell::sMFilter sMF(sCCI.scc_addIdInfo);
                std::shared_ptr<sLBuyCell::sMOffering> sMOff = sCA_MarketListing->getListOfOffering(sMF);

                //Creation of Contract:
                sCA_MainContractor->addContractToCompany(sCCI.scc_objCompany, sMOff->sMO_givingCompany, *sMOff, cDate);
                sCA_MarketListing->removeCompleteProcess(sMOff);
            }
                break;
            case sCompanyCompiler::sCCIntentions::CELL_GiveRent: {
                std::pair<int, int> gTile = sCCI.scc_objCompany->getOwnedByIndex(sCCI.scc_addIdInfo);
                std::shared_ptr<sLRentCell::sMOffering> sMO =
                        std::make_shared<sLRentCell::sMOffering>(sCCI.scc_objCompany, gTile,
                                                                 sCA_gType->gTG_TypeGen->get(gTile),
                                                                 (uint16_t) 0, (uint32_t) 0,
                                                                 con_TypePaymentFreq::LC_PAY_MONTH);
                sCA_MarketListing->addListing(sMO);
            }
                break;
            case sCompanyCompiler::sCCIntentions::CELL_GainRent: {
                sLRentCell::sMFilter sMF(sCCI.scc_addIdInfo);
                std::shared_ptr<sLRentCell::sMOffering> sMOff = sCA_MarketListing->getListOfOffering(sMF);

                sCA_MainContractor->addContractToCompany(sCCI.scc_objCompany, sMOff->sMO_givingCompany, *sMOff, cDate);
                sCA_MarketListing->removeCompleteProcess(sMOff);
            }
                break;
            case sCompanyCompiler::sCCIntentions::CELL_Sell: {
                std::pair<int, int> gTile = sCCI.scc_objCompany->getOwnedByIndex(sCCI.scc_addIdInfo);
                std::shared_ptr<sLBuyCell::sMOffering> sMO =
                        std::make_shared<sLBuyCell::sMOffering>(sCCI.scc_objCompany, gTile,
                                                                sCA_gType->gTG_TypeGen->get(gTile),
                                                                0, 0);
                sCA_MarketListing->addListing(sMO);
            }
                break;
            case sCompanyCompiler::sCCIntentions::OBJ_Produce:
                if (!gProduceProduct(sCCI.scc_objCompany, sCCI.scc_addIdInfo, sCodeStorage, cDate))
                    sCodeStorage->updateScoreCode(sCCI.scc_objCompany->c_uuid, -10);
                break;
            case sCompanyCompiler::sCCIntentions::OBJ_Buy:

                break;
        }
        return true;
    }

    void gCompletedProduct(const std::shared_ptr<objCompany> &oC, uint32_t gItemGen) {
        if (oC->c_pOwn.find(gItemGen) == oC->c_pOwn.end())
            oC->c_pOwn[gItemGen] = 1; //TO IMPROVE, potser fer-ne mes de un... cada cop
        else
            oC->c_pOwn[gItemGen] += 1;

        for (const auto &bRequired: sCA_CTR.getById(gItemGen).pr_reqBuilding)
            oC->c_cAvailableByType[bRequired] += 1;
    }

private:

    bool
    gProduceProduct(std::shared_ptr<objCompany> &oC, uint32_t gItemGen, std::shared_ptr<sCodeStorage> &sCodeStorage,
                    uint32_t cDate) {
        if (!hasTypeOwn(oC, gItemGen, sCA_CTR))
            return false;
        if (!hasResources(*oC, gItemGen, sCA_CTR)) {
            for (uint32_t mLack: sCA_CTR.getById(gItemGen).pr_reqProdId) {
                sCompanyCompiler::sCCIntentions buyItems = {
                        sCompanyCompiler::sCCIntentions::sCCEnumIntentions::OBJ_Buy, mLack, oC};
                gTryIntention(buyItems, sCodeStorage, cDate);
            }
        }

        for (const auto &gElem: sCA_CTR.getById(gItemGen).pr_reqProdId)
            oC->c_pOwn[gElem] -= 1;

        for (const auto &bRequired: sCA_CTR.getById(gItemGen).pr_reqBuilding)
            oC->c_cAvailableByType[bRequired] -= 1;

        sCA_gTimer->addTimer(gItemGen, sCA_CTR.getById(gItemGen).pr_reqTime + cDate, oC->c_uuid);
        return true;
    }

    //ELEMENTS VALIDATIONS;
    static bool hasTypeOwn(std::shared_ptr<objCompany> &oC, uint32_t gItemGen, sTotalRecipes &sTR) {
        std::map<uint32_t, uint8_t> gQuant; //TO IMPROVE;
        objProdRecipe oPR = sTR.getById(gItemGen);
        for (const auto &nObj: oPR.pr_reqBuilding)
            gQuant[nObj]++;

        for (const auto &bRequired: gQuant)
            if (oC->c_cAvailableByType.find(bRequired.first) == oC->c_cAvailableByType.end() ||
                oC->c_cAvailableByType[bRequired.first] < bRequired.second)
                return false;

        return true;
    }

    static bool hasResources(objCompany &oC, uint32_t gItemGen, sTotalRecipes &sTR) {
        std::map<uint32_t, uint8_t> gQuant; //TO IMPROVE;
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
    std::shared_ptr<gTerrainGrid> sCA_gType;
    std::shared_ptr<sCompanyTimer> sCA_gTimer;
    std::shared_ptr<sMarketBazaar> sCA_MarketListing;
    std::shared_ptr<sMainContractor> sCA_MainContractor;
};

#endif //CITYOFWEIRDFISHES_SCOMPANYACTIONS_H
