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

class sMCompany {
public:

    explicit sMCompany(uint32_t lSize, const std::shared_ptr<gIGrid<uint8_t>> &gTypeRef)
            : sCompRecipes(std::make_shared<sTotalRecipes>("FObjProduced.csv")) {
        gLayerOwnership = std::make_shared<gBasicGrid<std::list<uint32_t>>>
                (gBasicGrid<std::list<uint32_t>>(lSize, lSize, {}));
        sCompA = std::make_shared<sCompanyActions>(*sCompRecipes, gTypeRef, sCompT); //TODO TYPE
    }

    void tick(uint32_t tTime) {
        if (sCompT->hasToChange(tTime)) // Aixi no hem de crear un vector vuit cada frame.
            for (std::pair<uint32_t, uint32_t> &t: sCompT->checkForTime(tTime))
                sCompA->gCompletedProduct(sTComp->getCompanyByUUID(t.second), t.first);
        if (tTime % (3600 * 3) == 0) { //ONCE EVERY 3 HOURS
            for (sCompanyCompiler::sCCIntentions sCCI: sTComp->getTotalIntentions()) {
                sCompA->gTryIntention(sCCI);
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

    std::shared_ptr<sCompanyTimer> sCompT;
    std::shared_ptr<sCompanyActions> sCompA;
    std::shared_ptr<sTotalRecipes> sCompRecipes;
};

#endif //CITYOFWEIRDFISHES_SMCOMPANY_H
