//
// Created by Laminar on 17/03/2024.
//

#ifndef CITYOFWEIRDFISHES_OBJCOMPANY_H
#define CITYOFWEIRDFISHES_OBJCOMPANY_H


#include <cstdint>
#include <utility>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <unordered_set>

std::vector<std::string> vSyllablesP = {"am", "ca", "mi", "o", "ul", "er", "es", "pin", "tu", "ra", "ta", "la", "dro",
                                        "me", "dia", "mart", "sen", "ti", "ments", "tran", "qui", "li", "tat", "pen",
                                        "sa", "ment", "u", "su", "al", "ment", "ar", "ma", "ri"};
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> numStringsDist(2, 4);

class objCompany {
public:
    struct objComp_activeDates {
        uint8_t cAD_jobWeek;
        std::pair<uint32_t, uint32_t> c_StrEndTime;

        objComp_activeDates(const std::vector<bool> &cAdJobWeek, const std::pair<uint32_t, uint32_t> &cStrEndTime)
                : c_StrEndTime(cStrEndTime) {
            if (cAdJobWeek.size() != 7)
                throw std::invalid_argument("The input bool vector must have exactly 7 elements.");

            cAD_jobWeek = 0;
            for (size_t i = 0; i < 7; ++i)
                if (cAdJobWeek[i])
                    cAD_jobWeek |= (1u << i);
        }
    };

    explicit objCompany(uint32_t cUuid, const std::list<std::pair<int, int>> &ownStart,
                        objComp_activeDates activeDates)
            : c_uuid(cUuid), c_cActiveLocations(ownStart), c_activeDates(std::move(activeDates)) {
        int numStrings = numStringsDist(gen);
        std::vector<std::string> selectedStrings;
        std::sample(vSyllablesP.begin(), vSyllablesP.end(),
                    std::back_inserter(selectedStrings), numStrings, gen);

        for (const auto &str: selectedStrings)
            nName += str;
        nName[0] = toupper(nName[0]);
        c_cCode = {13835058055282163712UL};
    }


    void addAsReceiving(con_rentCell &rentCell) {
        //Gain to active
        con_lRentC.push_back(rentCell);
        c_cActiveLocations.insert(c_cActiveLocations.end(), rentCell.ct_lCells.begin(), rentCell.ct_lCells.end());
        addPropertyContract(rentCell.ct_typePayment, rentCell.ct_recurrentCost, false);
    }

    void addAsGiving(con_rentCell &rentCell) {
        //Remove from active
        con_lRentC.push_back(rentCell);
        for (const auto &item: rentCell.ct_lCells) {
            c_cActiveLocations.erase(std::remove(c_cActiveLocations.begin(), c_cActiveLocations.end(), item),
                                     c_cActiveLocations.end());
        }
        //Positive workflow timer
        addPropertyContract(rentCell.ct_typePayment, rentCell.ct_recurrentCost, true);
    }


    void addAsReceiving(con_buyCell &buyCell) {
        con_lBuyC.push_back(buyCell);
        c_cActiveLocations.insert(c_cActiveLocations.end(), buyCell.ct_lCells.begin(), buyCell.ct_lCells.end());
        //Negative workflow timer
        addPropertyContract(buyCell.ct_typePayment, buyCell.ct_recurrentCost, false);
    }

    void addAsGiving(con_buyCell &buyCell) {
        con_lBuyC.push_back(buyCell);
        for (const auto &item: buyCell.ct_lCells) {
            c_cActiveLocations.erase(std::remove(c_cActiveLocations.begin(), c_cActiveLocations.end(), item),
                                     c_cActiveLocations.end());
        }
        //Positive workflow timer
        addPropertyContract(buyCell.ct_typePayment, buyCell.ct_recurrentCost, true);
    }

    void removeAsReceiving(con_rentCell &rentCell) {
        con_lRentC.remove(rentCell);
        for (const auto &item: rentCell.ct_lCells) {
            c_cActiveLocations.erase(std::remove(c_cActiveLocations.begin(), c_cActiveLocations.end(), item),
                                     c_cActiveLocations.end());
        }
        addPropertyContract(rentCell.ct_typePayment, rentCell.ct_recurrentCost, true);
    }

    void removeAsGiving(con_rentCell &rentCell) {
        con_lRentC.remove(rentCell);
        c_cActiveLocations.insert(c_cActiveLocations.end(), rentCell.ct_lCells.begin(), rentCell.ct_lCells.end());
        //Negative workflow timer
        addPropertyContract(rentCell.ct_typePayment, rentCell.ct_recurrentCost, false);
    }

    void removeAsReceiving(con_buyCell &buyCell) {
        con_lBuyC.remove(buyCell);
        for (const auto &item: buyCell.ct_lCells) {
            c_cActiveLocations.erase(std::remove(c_cActiveLocations.begin(), c_cActiveLocations.end(), item),
                                     c_cActiveLocations.end());
        }
        addPropertyContract(buyCell.ct_typePayment, buyCell.ct_recurrentCost, true);
    }

    void removeAsGiving(con_buyCell &buyCell) {
        con_lBuyC.remove(buyCell);
        c_cActiveLocations.insert(c_cActiveLocations.end(), buyCell.ct_lCells.begin(), buyCell.ct_lCells.end());
        //Negative workflow timer
        addPropertyContract(buyCell.ct_typePayment, buyCell.ct_recurrentCost, false);
    }

    uint32_t c_uuid{};
    std::string nName;

    std::pair<int, int> getOwnedByIndex(uint32_t lIndex) {
        return *std::next(c_cActiveLocations.begin(),
                          lIndex % c_cActiveLocations.size()); // Advance iterator by 2 positions
    }


    std::map<uint32_t, int> c_pOwn;
    std::list<std::pair<int, int>> c_cActiveLocations;

protected:

    //Easy Look-ups
    objComp_activeDates c_activeDates;
    uint32_t c_objFortnight = 0;
    uint32_t c_objYear = 0;
    uint32_t c_objMonth = 0;

    //Active Contracts;
    std::list<con_rentCell> con_lRentC;
    std::list<con_buyCell> con_lBuyC;
    std::list<con_loanInteraction> con_lLoanI;
    std::list<con_stockInteraction> con_lStockI;

    void addPropertyContract(con_TypePaymentFreq c_TPF, uint32_t ct_recurrentCost, bool isGaining) {
        switch (c_TPF) {
            case LC_PAY_MONTH:
                c_objMonth = isGaining ? c_objMonth + ct_recurrentCost : c_objMonth - ct_recurrentCost;
                break;
            case LC_PAY_YEAR:
                c_objYear = isGaining ? c_objYear + ct_recurrentCost : c_objYear - ct_recurrentCost;
                break;
            case LC_PAY_FORTNIGHT:
                c_objFortnight = isGaining ? c_objFortnight + ct_recurrentCost : c_objFortnight - ct_recurrentCost;
                break;
        }
    }
};

#endif //CITYOFWEIRDFISHES_OBJCOMPANY_H
