//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMPANYMAIN_H
#define CITYOFWEIRDFISHES_SCOMPANYMAIN_H

#include "sCompanyTimer.h"
#include "sCompanyActions.h"

#include "sCompanyStorage.h"
#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"
#include "employee/sCivilMain.h"
#include "../sgTerrain.h"
#include "code/sCodeStoratge.h"
#include "../market/sEvaluatorMain.h"

class sCompanyMain {
public:

    enum sCM_strStyleCompany {
        SCM_strOffice = 0, SCM_strFactory = 1, SCM_strField = 2
    };

    explicit sCompanyMain(uint32_t inGridSize,
                          const std::shared_ptr<sCivilMain> &inPCivilMain,
                          const std::shared_ptr<sgTerrain> &inPSGTerrain,
                          const std::shared_ptr<sMarketBazaar> &inSMarketBazaar,
                          const std::shared_ptr<gIGrid<uint8_t>> &inPGridTypeAir,
                          const std::shared_ptr<sEvaluatorMain> &inPSEvaluatorMain)
            : sM_gMainTerrain(inPSGTerrain), sM_AirCondition(inPGridTypeAir), sM_sCompEmployee(inPCivilMain),
              sM_sMarketBazaar(inSMarketBazaar) {
        sCompT = std::make_shared<sCompanyTimer>();
        gLayerOwnership = std::make_shared<gBasicGrid<std::list<uint32_t>>>
                (gBasicGrid<std::list<uint32_t>>(inGridSize, inGridSize, {}));
        sCompA = std::make_shared<sCompanyActions>(sM_gMainTerrain, inSMarketBazaar, sCompT, inPSEvaluatorMain);
    }

    void tickReduced(const uint32_t inRTime, const uint32_t inTDate) {
        if (sCompT->hasToChange(inTDate)) // Aixi no hem de crear un vector vuit cada frame.
            for (std::pair<uint32_t, uint32_t> &t: sCompT->checkForTime(inTDate))
                sCompA->gCompletedProduct(sTComp->getCompanyByUUID(t.second), t.first);
        if (inRTime % (12 * 3) == 0) { //ONCE EVERY 3 HOURS
            for (sCompanyCompiler::sCCIntentions sCCI: sTComp->getTotalIntentions(inRTime)) {
                sCompA->gTryIntention(sCCI, sSCode, inTDate);
                sSCode->updateScoreCode(sCCI.scc_objCompany->c_uuid, -1);
            }
        }

        std::vector<std::pair<uint32_t, int>> sDiffEmp = sTComp->getDiffEmployeesByLocation(inRTime);
        for (const std::pair<uint32_t, int> &dEmp: sDiffEmp) {
            if (dEmp.second > 0)
                for (int i = 0; i < dEmp.second; i++)
                    addEmployee(sTComp->getCompanyByUUID(dEmp.first));
            else
                for (int i = 0; i < abs(dEmp.second); i++)
                    removeEmployee(sTComp->getCompanyByUUID(dEmp.first));
        }

        if (inRTime == 0) {
            if ((inTDate % 1 << 9) == 0)
                sTComp->applyEcoYear();
            if ((inTDate % 1 << 5) == 0)
                sTComp->applyEcoMonth();
            if ((inTDate % 1 << 3) == 0)
                sTComp->applyEcoWeek();
        }
    }

    void completedStartCompanies(const std::vector<retObjCompany> &gPosCompanies) {
        for (const auto &posNewComp: gPosCompanies) {
            uint32_t uuidNew = sTComp->addCompanyAtPosition(gLayerOwnership,
                                                            std::list<std::pair<int, int>>
                                                                    (posNewComp.gCompVec.begin(),
                                                                     posNewComp.gCompVec.end()),
                                                            posNewComp.gType);
            for (int i = 0; i < 2; i++)
                addEmployee(sTComp->getCompanyByUUID(uuidNew));
        }
    }

    void addNewCompany(sCM_strStyleCompany cCompanyCreation) {
        std::vector<sgTerrain::sgT_TypeGen> gTypeGivenTC = {sgTerrain::sgT_TypeGen::sgT_TG_CivBuilding,
                                                            sgTerrain::sgT_TypeGen::sgT_TG_IndBuilding,
                                                            sgTerrain::sgT_TypeGen::sgT_TG_FieldBuilding};
        sgTerrain::sgT_TypeGen gTypeCompany = gTypeGivenTC[cCompanyCreation];

        std::shared_ptr<sLBuyCell::sMOffering> gOffer = sM_sMarketBazaar->getListOfOffering(
                sLBuyCell::sMFilter(gTypeCompany));
        uint32_t uuidNew = sTComp->addCompanyAtPosition(gLayerOwnership, {gOffer->sMO_pos}, gTypeCompany);
        sM_sMarketBazaar->removeCompleteProcess(gOffer);

        sM_gMainTerrain->addNewBuilding(gTypeCompany, gOffer->sMO_pos);
        for (int i = 0; i < 2; i++)
            addEmployee(sTComp->getCompanyByUUID(uuidNew));
    }

    //STORAGE
    std::shared_ptr<gIGrid<std::list<uint32_t>>> gLayerOwnership;

    std::shared_ptr<sCodeStorage> sSCode = std::make_shared<sCodeStorage>();
    std::shared_ptr<sCompanyStorage> sTComp = std::make_shared<sCompanyStorage>(1000, sSCode);
private:


    void addEmployee(const std::shared_ptr<objCompany> &inCompany) {
        std::pair<int, int> pHouseEmployee = sM_sCompEmployee->setRouteToNewEmployee(*inCompany);
        inCompany->c_objMonth -= 300; //SALARI
        inCompany->c_nEmployee += 1;
        if (!gLayerOwnership->get(pHouseEmployee).empty())
            sTComp->getCompanyByUUID(gLayerOwnership->get(pHouseEmployee).front())->c_objMonth += 150; //RENT
    }

    void removeEmployee(const std::shared_ptr<objCompany> &inCompany) {
        std::pair<int, int> pHouseEmployee =  sM_sCompEmployee->removeEmployeeToCompany(*inCompany);
        inCompany->c_objMonth += 300; //SALARI
        inCompany->c_nEmployee -= 1;
        if (!gLayerOwnership->get(pHouseEmployee).empty())
            sTComp->getCompanyByUUID(gLayerOwnership->get(pHouseEmployee).front())->c_objMonth -= 150; //RENT
    }

    std::shared_ptr<sgTerrain> sM_gMainTerrain;
    std::shared_ptr<gIGrid<uint8_t>> sM_AirCondition;

    std::shared_ptr<sCompanyTimer> sCompT;
    std::shared_ptr<sCompanyActions> sCompA;
    std::shared_ptr<sCivilMain> sM_sCompEmployee;

    std::shared_ptr<sMarketBazaar> sM_sMarketBazaar;
};

#endif //CITYOFWEIRDFISHES_SCOMPANYMAIN_H