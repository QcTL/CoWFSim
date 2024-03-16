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

class sMCompany {
public:

    explicit sMCompany(uint32_t lSize,
                       const std::shared_ptr<gIGrid<uint8_t>> &gTypeRef,
                       const std::shared_ptr<gIGrid<uint8_t>> &gTypeSoil,
                       const std::shared_ptr<gIGrid<uint8_t>> &gTypeAir)
            : sCompRecipes(std::make_shared<sTotalRecipes>("FObjProduced.csv")),
              sM_TRef(gTypeRef), sM_TSoil(gTypeSoil), sM_AirCondition(gTypeAir) {

        sCompT = std::make_shared<sCompanyTimer>();
        gLayerOwnership = std::make_shared<gBasicGrid<std::list<uint32_t>>>
                (gBasicGrid<std::list<uint32_t>>(lSize, lSize, {}));
        sCompA = std::make_shared<sCompanyActions>(*sCompRecipes, gTypeRef, sCompT);
    }

    void tick(uint32_t tTime) {
        if (sCompT->hasToChange(tTime)) // Aixi no hem de crear un vector vuit cada frame.
            for (std::pair<uint32_t, uint32_t> &t: sCompT->checkForTime(tTime))
                sCompA->gCompletedProduct(sTComp->getCompanyByUUID(t.second), t.first);
        std::cout << tTime << std::endl;
        if (tTime % (12 * 3) == 0) { //ONCE EVERY 3 HOURS
            for (sCompanyCompiler::sCCIntentions sCCI: sTComp->getTotalIntentions()) {
                sCompA->gTryIntention(sCCI, sM_AirCondition, sM_TSoil, tTime);
                //TODO restar score si ho fa malament; sumar una mica si ho fa be
            }
        }

    }

    void completedStartCompanies(const std::vector<std::vector<std::pair<int, int>>> &gPosCompanies) {
        for (const auto &posNewComp: gPosCompanies)
            sTComp->addCompanyAtPosition(gLayerOwnership, posNewComp);
    }

    std::shared_ptr<gIGrid<std::list<uint32_t>>> gLayerOwnership;
    std::shared_ptr<sTotalCompany> sTComp = std::make_shared<sTotalCompany>(1000);
private:

    std::shared_ptr<gIGrid<uint8_t>> sM_TRef;
    std::shared_ptr<gIGrid<uint8_t>> sM_TSoil;
    std::shared_ptr<gIGrid<uint8_t>> sM_AirCondition;

    std::shared_ptr<sCompanyTimer> sCompT;
    std::shared_ptr<sCompanyActions> sCompA;
    std::shared_ptr<sTotalRecipes> sCompRecipes;
    sTotalEmployee sCompEmployee;
};

#endif //CITYOFWEIRDFISHES_SMCOMPANY_H
