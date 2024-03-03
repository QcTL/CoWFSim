//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMMON_H
#define CITYOFWEIRDFISHES_SCOMMON_H


#include <cstdint>
#include <utility>
#include <list>
#include <vector>
#include <map>
#include <string>


struct ctrct_leaseCell {
    enum TypePayment {
        LC_PAY_MONTH, LC_PAY_YEAR, LC_PAY_ONCE
    };
    uint32_t ct_strDate;
    uint32_t ct_endDate;
    TypePayment ct_typePayment;
    uint32_t ct_totalCost;
    std::vector<std::pair<uint32_t, uint32_t>> ct_lCells;
};

std::vector<std::string> vNames = {"Whiterock", "Zunzon", "TentAroma", "Prova"};

struct objCompany {
    uint32_t c_uuid{};
    std::string nName;
    std::vector<std::pair<uint32_t, uint32_t>> c_cOwn;
    std::list<uint32_t> c_cRentedSelf;
    std::list<ctrct_leaseCell> c_cRentedOther;
    std::map<uint32_t, int> c_pOwn;

    objCompany() = default;

    explicit objCompany(uint32_t cUuid) : c_uuid(cUuid) {
        nName = vNames[cUuid % vNames.size()];
    }
};

struct objRoadTravel {
    std::pair<int, int> c_RStart;
    std::pair<int, int> c_REnd;
};

struct objCivil {
    objRoadTravel c_Travel;

    uint32_t c_TBegin;
    uint32_t c_TEnd;
};


#endif //CITYOFWEIRDFISHES_SCOMMON_H
