//
// Created by Laminar on 17/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCONTRACTORTIMER_H
#define CITYOFWEIRDFISHES_SCONTRACTORTIMER_H

#include <list>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <memory>
#include "../../../structure/obj/elements/objCompany.h"

class sContractorTimer {
public:

    sContractorTimer() = default;

    void addTimer(uint64_t uuidContract, uint32_t gTimer,
                  const std::shared_ptr<objCompany> &givingCompany,
                  const std::shared_ptr<objCompany> &receivingCompany) {
        sObjContractTimer element = {gTimer, uuidContract, givingCompany, receivingCompany};
        gListTimers.insert(std::find_if(gListTimers.begin(), gListTimers.end(), [&](const auto &item) {
            return item.sOT_Timer >= element.sOT_Timer;
        }), element);
    }

    bool hasToChange(uint32_t gTimer) {
        if (gListTimers.empty())
            return false;
        return gListTimers.front().sOT_Timer < gTimer;
    };

    void callToProcessEndedContracts(uint32_t gTimer) {
        while (gListTimers.front().sOT_Timer < gTimer) {
            gListTimers.front().sOT_GivingCompany->removeAsGiving(SOMEFUCKIINGWAYOFGETTINGCONTRACTSBYUIDDANDCASTIT(gListTimers.front().sOT_uuidContract));
            gListTimers.front().sOT_ReceivingCompany->removeAsReceiving(SOMEFUCKIINGWAYOFGETTINGCONTRACTSBYUIDDANDCASTIT(gListTimers.front().sOT_uuidContract));
            gListTimers.pop_front();
        }
    }

private:
    struct sObjContractTimer {
        uint32_t sOT_Timer;
        uint64_t sOT_uuidContract;
        std::shared_ptr<objCompany> sOT_GivingCompany;
        std::shared_ptr<objCompany> sOT_ReceivingCompany;
    };

    std::list<sObjContractTimer> gListTimers;
};

#endif //CITYOFWEIRDFISHES_SCONTRACTORTIMER_H
