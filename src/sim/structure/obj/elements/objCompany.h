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
#include "../../../eyeCatcher/eyeValue.h"

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

    explicit objCompany(const std::list<std::pair<int, int>> &ownStart, uint8_t typeCellStart,
                        objComp_activeDates activeDates)
            : c_activeDates(std::move(activeDates)) {

        addAvailableLocation(ownStart, typeCellStart);
        int numStrings = numStringsDist(oC_gen);
        std::vector<std::string> selectedStrings;
        std::sample(vSyllablesP.begin(), vSyllablesP.end(),
                    std::back_inserter(selectedStrings), numStrings, oC_gen);

        for (const auto &str: selectedStrings)
            nName += str;
        nName[0] = static_cast<char>(toupper(nName[0]));

        c_nEmployee = eyeValue<uint32_t>("oC_" + nName + "-nEmployees", 0);
        c_cActiveFunds = eyeValue<double>("oC_" + nName + "-activeFunds", 0);
        c_objWeek = eyeValue<float>("oC_" + nName + "-gainWeek", 0);
        c_objYear = eyeValue<float>("oC_" + nName + "-gainYear", 0);
        c_objMonth = eyeValue<float>("oC_" + nName + "-gainMonth", 0);
    }


    void addAvailableLocation(const std::list<std::pair<int, int>> &ctLCells, uint8_t typeSoilCell) {
        c_cActiveLocations[typeSoilCell].insert(
                c_cActiveLocations[typeSoilCell].end(), ctLCells.begin(), ctLCells.end());
        for (int i = 0; i < ctLCells.size(); i++) {
            if (c_cAvailableByType.find(typeSoilCell) != c_cAvailableByType.end())
                c_cAvailableByType[typeSoilCell] += 1;
            else
                c_cAvailableByType[typeSoilCell] = 1;
        }
    }

    void removeAvailableLocation(const std::list<std::pair<int, int>> &ctLCells, uint8_t typeCell) {
        for (const auto &item: ctLCells) {
            c_cActiveLocations[typeCell].erase(
                    std::remove(c_cActiveLocations[typeCell].begin(), c_cActiveLocations[typeCell].end(), item),
                    c_cActiveLocations[typeCell].end());
        }
        c_cAvailableByType[typeCell] -= 1;
    }

    void addRentedLocationByPos(const std::pair<int, int> inPRented, uint8_t typeCell) {
        removeAvailableLocation({inPRented}, typeCell);
        c_cRentedLocations[typeCell].push_back(inPRented);
    }

    uint32_t c_uuid{};
    std::string nName;

    [[nodiscard]] bool hasOwnedType(uint8_t inNType) const {
        return !c_cActiveLocations.at(inNType).empty();
    }

    [[nodiscard]] uint32_t getNumberRentedCells() const {
        size_t totalLength = 0;
        for (const auto &pair: c_cRentedLocations)
            totalLength += pair.second.size();
        return totalLength;
    }

    [[nodiscard]] uint32_t getNumberActiveCells() const {
        size_t totalLength = 0;
        for (const auto &pair: c_cActiveLocations)
            totalLength += pair.second.size();
        return totalLength;
    }

    std::pair<int, int> getOwnedByType(uint8_t inNType) {
        return c_cActiveLocations[inNType].front();
    }

    void addContractToCompany(uint8_t inCType, uint64_t inCUuid) {
        if (c_activeContracts.find(inCType) != c_activeContracts.end())
            c_activeContracts[inCType].push_back(inCUuid);
        else
            c_activeContracts[inCType] = {inCUuid};
    }

    uint8_t getTypeByIndex(uint32_t inIndexCell) {
        size_t remainingIndex = inIndexCell;
        for (const auto &pair: c_cActiveLocations) {
            const auto &list = pair.second;
            size_t listSize = list.size();

            if (remainingIndex < listSize) {
                auto it = list.begin();
                std::advance(it, remainingIndex);
                return pair.first;
            }
            remainingIndex -= listSize;
        }
        return 0;
    }

    void addPayment(double inQuantityPayment, const oPC_TypePayment inTypePayment, uint32_t inRTime, uint32_t inTDate) {
        c_listPayments.push_back({inQuantityPayment, inTypePayment, inRTime, inTDate});
        c_cActiveFunds += inQuantityPayment;
    }

    enum objC_TimeObligations {
        objC_TO_WEEK, objC_TO_MONTH, objC_TO_YEAR
    };

    void complyTimeObligations(const objC_TimeObligations inTimeType, const uint32_t inTDate) {
        switch (inTimeType) {
            case objC_TO_WEEK:
                c_listPayments.push_back({c_objWeek, oPC_TypePayment::oPC_TP_RECURRENT_WEEK, 0, inTDate});
                c_cActiveFunds += c_objWeek;
                break;
            case objC_TO_MONTH:
                c_listPayments.push_back({c_objMonth, oPC_TypePayment::oPC_TP_RECURRENT_MONTH, 0,inTDate});
                c_cActiveFunds += c_objMonth;
                break;
            case objC_TO_YEAR:
                c_listPayments.push_back({c_objYear, oPC_TypePayment::oPC_TP_RECURRENT_YEAR, 0, inTDate});
                c_cActiveFunds += c_objYear;
                break;
        }
    }

    [[nodiscard]] std::vector<objPaymentCompany> getVecAllTransactions() const{
        return c_listPayments;
    }

    std::map<uint32_t, int> c_pOwn;
    std::map<uint8_t, std::list<std::pair<int, int>>> c_cActiveLocations;
    std::map<uint8_t, int> c_cAvailableByType;
    std::map<uint8_t, std::list<std::pair<int, int>>> c_cRentedLocations;
    objComp_activeDates c_activeDates;

    eyeValue<uint32_t> c_nEmployee;
    eyeValue<float> c_objWeek;
    eyeValue<float> c_objYear;
    eyeValue<float> c_objMonth;

    std::shared_ptr<sCodeObj> c_cCode;
    std::vector<objPaymentCompany> c_listPayments;
    std::map<uint8_t, std::list<uint64_t>> c_activeContracts;
    bool c_attrCanEmployee = true;

    eyeValue<double> c_cActiveFunds;
private:
    static std::vector<std::string> vSyllablesP;
    static std::random_device oC_rd;
    static std::mt19937 oC_gen;
    static std::uniform_int_distribution<> numStringsDist;
};

#endif //CITYOFWEIRDFISHES_OBJCOMPANY_H
