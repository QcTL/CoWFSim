//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SMCOMPANY_H
#define CITYOFWEIRDFISHES_SMCOMPANY_H

#include "sCompanyTimer.h"
#include "sCompanyActions.h"

#include "../../structure/obj/sTotalCompany.h"
#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"
#include "employee/sMainCivil.h"
#include "../gTerrainGrid.h"
#include "code/sCodeStoratge.h"

class sMCompany {
public:

    enum sMComp_TypeCompany {
        SMComp_Office = 0, SMComp_Factory = 1, SMComp_Field = 2
    };

    explicit sMCompany(uint32_t lSize,
                       const std::shared_ptr<sMainCivil> &sMainCivil,
                       const std::shared_ptr<gTerrainGrid> &gTerrainGrid,
                       const std::shared_ptr<gIGrid<uint8_t>> &gTypeAir)
            : sCompRecipes(std::make_shared<sTotalRecipes>("FObjProduced.csv")),
              sM_gMainTerrain(gTerrainGrid), sM_AirCondition(gTypeAir), sM_sCompEmployee(sMainCivil) {

        sCompT = std::make_shared<sCompanyTimer>();
        gLayerOwnership = std::make_shared<gBasicGrid<std::list<uint32_t>>>
                (gBasicGrid<std::list<uint32_t>>(lSize, lSize, {}));
        sCompA = std::make_shared<sCompanyActions>(*sCompRecipes, sM_gMainTerrain, sCompT);
    }

    void tickReduced(const uint32_t tTime, const uint32_t cDate) {
        if (sCompT->hasToChange(cDate)) // Aixi no hem de crear un vector vuit cada frame.
            for (std::pair<uint32_t, uint32_t> &t: sCompT->checkForTime(cDate))
                sCompA->gCompletedProduct(sTComp->getCompanyByUUID(t.second), t.first);
        std::cout << tTime << std::endl;
        if (tTime % (12 * 3) == 0) { //ONCE EVERY 3 HOURS
            for (sCompanyCompiler::sCCIntentions sCCI: sTComp->getTotalIntentions(sSCode)) {
                sCompA->gTryIntention(sCCI, sSCode, cDate);
                sSCode->updateScoreCode(sCCI.scc_objCompany->c_uuid, -1);
            }
        }

        if (tTime == 0 && (cDate % 1 << 9) == 0) {
            sTComp->applyEcoWeek();
            sTComp->applyEcoMonth();
            sTComp->applyEcoYear();
        } else if (tTime == 0 && (cDate % 1 << 5) == 0) {
            sTComp->applyEcoWeek();
            sTComp->applyEcoMonth();
        } else if (tTime == 0 && (cDate % 1 << 3) == 0) {
            sTComp->applyEcoWeek();
        }

    }

    void completedStartCompanies(const std::vector<std::vector<std::pair<int, int>>> &gPosCompanies) {
        for (const auto &posNewComp: gPosCompanies)
            sTComp->addCompanyAtPosition(gLayerOwnership,
                                         std::list<std::pair<int, int>>(posNewComp.begin(), posNewComp.end()));
    }

    void addNewCompany(sMComp_TypeCompany cCompanyCreation) {
        std::vector<uint8_t> gTypeGivenTC = {1, 2, 4};
        uint8_t gTypeCompany = gTypeGivenTC[cCompanyCreation];

        auto itNewPos = sM_gMainTerrain->getEmptyPositionByType(gTypeCompany);
        sTComp->addCompanyAtPosition(gLayerOwnership, {*itNewPos});
        sM_gMainTerrain->removeEmptyPositionByIterator(gTypeCompany, itNewPos);
    }

    //STORAGE
    std::shared_ptr<gIGrid<std::list<uint32_t>>> gLayerOwnership;
    std::shared_ptr<sTotalCompany> sTComp = std::make_shared<sTotalCompany>(1000);
    std::shared_ptr<sCodeStorage> sSCode = std::make_shared<sCodeStorage>();

private:
    std::shared_ptr<gTerrainGrid> sM_gMainTerrain;
    std::shared_ptr<gIGrid<uint8_t>> sM_AirCondition;

    std::shared_ptr<sCompanyTimer> sCompT;
    std::shared_ptr<sCompanyActions> sCompA;
    std::shared_ptr<sTotalRecipes> sCompRecipes;
    std::shared_ptr<sMainCivil> sM_sCompEmployee;
};

#endif //CITYOFWEIRDFISHES_SMCOMPANY_H