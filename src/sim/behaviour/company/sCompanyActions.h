//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMPANYACTIONS_H
#define CITYOFWEIRDFISHES_SCOMPANYACTIONS_H

#include <set>
#include <unordered_set>
#include "../../structure/grids/gIGrid.h"
#include "sCompanyTimer.h"
#include "../market/sMarketBazaar.h"
#include "../sgTerrain.h"
#include "contract/sContractorMain.h"
#include "sCompanyCompiler.h"
#include "../market/sEvaluatorMain.h"

class sCompanyActions {
public:
    sCompanyActions(const std::shared_ptr<sgTerrain> &gType,
                    const std::shared_ptr<sCompanyTimer> &gCTimer,
                    const std::shared_ptr<sEvaluatorMain> &sMEvaluator)
            : sCA_gTimer(gCTimer), sCA_gType(gType), sMEvaluator(sMEvaluator) {}

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
                sMEvaluator->computeBoughtElement(sCCI.scc_addIdInfo, sCCI.scc_objCompany);
                break;
            case sCompanyCompiler::sCCIntentions::OBJ_SellInm:
                sMEvaluator->computeSellInmElement(sCCI.scc_addIdInfo, sCCI.scc_objCompany);
                break;
        }
        return true;
    }

    void gCompletedProduct(const std::shared_ptr<objCompany> &oC, uint32_t gItemGen) {
        sMEvaluator->computeCreatedElement(gItemGen, oC);
        oC->c_cAvailableByType[sMEvaluator->getById(gItemGen).sMEE_iReqTypeBuild] += 1;
    }

private:

    bool
    gProduceProduct(std::shared_ptr<objCompany> &oC, uint32_t gItemGen, std::shared_ptr<sCodeStorage> &sCodeStorage,
                    uint32_t cDate) {
        if (!hasTypeOwn(oC, gItemGen))
            return false;
        if (!hasResources(*oC, gItemGen))
            for (uint32_t mLack: sMEvaluator->getById(gItemGen).sMEE_iCElem) {
                sCompanyCompiler::sCCIntentions buyItems = {
                        sCompanyCompiler::sCCIntentions::sCCEnumIntentions::OBJ_Buy, mLack, oC};
                gTryIntention(buyItems, sCodeStorage, cDate);
            }

        for (const auto &gElem: sMEvaluator->getById(gItemGen).sMEE_iCElem)
            oC->c_pOwn[gElem] -= 1;
        oC->c_cAvailableByType[sMEvaluator->getById(gItemGen).sMEE_iReqTypeBuild] -= 1;
        sCA_gTimer->addTimer(gItemGen, sMEvaluator->getById(gItemGen).sMEE_iTime + cDate, oC->c_uuid);
        std::cout << "CREATED PRODUCT" << std::endl;
        return true;
    }

    //ELEMENTS VALIDATIONS;
    bool hasTypeOwn(std::shared_ptr<objCompany> &oC, uint32_t gItemGen) {
        sTotalElements::sME_Element oPR = sMEvaluator->getById(gItemGen);
        if (oC->c_cAvailableByType.find(oPR.sMEE_iReqTypeBuild) == oC->c_cAvailableByType.end())
            return false;
        return oC->c_cAvailableByType[oPR.sMEE_iReqTypeBuild] > 0;
    }

    bool hasResources(objCompany &oC, uint32_t gItemGen) {
        std::map<uint32_t, uint8_t> gQuant; //TO IMPROVE;
        sTotalElements::sME_Element oPR = sMEvaluator->getById(gItemGen);
        for (const auto &nObj: oPR.sMEE_iCElem)
            gQuant[nObj]++;

        for (const auto &pair: gQuant) {
            if (oC.c_pOwn.find(pair.first) == oC.c_pOwn.end() || oC.c_pOwn.at(pair.first) < pair.second)
                return false;
        }
        return true;
    }

    std::shared_ptr<sEvaluatorMain> sMEvaluator;
    std::shared_ptr<sgTerrain> sCA_gType;
    std::shared_ptr<sCompanyTimer> sCA_gTimer;
    std::shared_ptr<sMarketBazaar> sCA_MarketListing;
    std::shared_ptr<sContractorMain> sCA_MainContractor;
};

#endif //CITYOFWEIRDFISHES_SCOMPANYACTIONS_H
