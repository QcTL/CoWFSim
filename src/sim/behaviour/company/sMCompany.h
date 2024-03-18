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
#include "employee/sTotalEmployee.h"
#include "../gTerrainGrid.h"

class sMCompany {
public:

    explicit sMCompany(uint32_t lSize,
                       const std::shared_ptr<gTerrainGrid> &gTerrainGrid,
                       const std::shared_ptr<gIGrid<uint8_t>> &gTypeAir)
            : sCompRecipes(std::make_shared<sTotalRecipes>("FObjProduced.csv")),
              sM_gMainTerrain(gTerrainGrid), sM_AirCondition(gTypeAir) {

        sCompT = std::make_shared<sCompanyTimer>();
        gLayerOwnership = std::make_shared<gBasicGrid<std::list<uint32_t>>>
                (gBasicGrid<std::list<uint32_t>>(lSize, lSize, {}));
        sCompA = std::make_shared<sCompanyActions>(*sCompRecipes, sM_gMainTerrain->gTG_TypeGen, sCompT);
    }

    void tick(uint32_t tTime) {
        if (sCompT->hasToChange(tTime)) // Aixi no hem de crear un vector vuit cada frame.
            for (std::pair<uint32_t, uint32_t> &t: sCompT->checkForTime(tTime))
                sCompA->gCompletedProduct(sTComp->getCompanyByUUID(t.second), t.first);
        std::cout << tTime << std::endl;
        if (tTime % (12 * 3) == 0) { //ONCE EVERY 3 HOURS
            for (sCompanyCompiler::sCCIntentions sCCI: sTComp->getTotalIntentions()) {
                sCompA->gTryIntention(sCCI, sM_AirCondition, sM_gMainTerrain, tTime);
                //TODO restar score si ho fa malament; sumar una mica si ho fa be
            }
        }
    }

    void completedStartCompanies(const std::vector<std::vector<std::pair<int, int>>> &gPosCompanies) {
        for (const auto &posNewComp: gPosCompanies)
            sTComp->addCompanyAtPosition(gLayerOwnership, posNewComp);
    }

    void addNewCompany() {
        auto itNewPos = sM_gMainTerrain->getEmptyPositionByType(1);
        sTComp->addCompanyAtPosition(gLayerOwnership, {*itNewPos});
        sM_gMainTerrain->removeEmptyPositionByIterator(1, itNewPos);
    }

    std::shared_ptr<gIGrid<std::list<uint32_t>>> gLayerOwnership;
    std::shared_ptr<sTotalCompany> sTComp = std::make_shared<sTotalCompany>(1000);
private:
    std::shared_ptr<gTerrainGrid> sM_gMainTerrain;
    std::shared_ptr<gIGrid<uint8_t>> sM_AirCondition;

    std::shared_ptr<sCompanyTimer> sCompT;
    std::shared_ptr<sCompanyActions> sCompA;
    std::shared_ptr<sTotalRecipes> sCompRecipes;
    sTotalEmployee sCompEmployee;
};

#endif //CITYOFWEIRDFISHES_SMCOMPANY_H