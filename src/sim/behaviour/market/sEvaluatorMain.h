//
// Created by ganymede on 3/21/24.
//

#ifndef CITYOFWEIRDFISHES_SEVALUATORMAIN_H
#define CITYOFWEIRDFISHES_SEVALUATORMAIN_H

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "items/sRollingListsEvaluator.h"
#include "items/sTotalElements.h"
#include "items/sProductTimer.h"

class sEvaluatorMain {
public:
    sEvaluatorMain() {
        sEM_totalElements = std::make_shared<sTotalElements>("FObjProduced.csv");


        sEM_gBasicPrices = std::vector<uint32_t>(sEM_totalElements->nElements(), 100);
        for (int i = 0; i < sEM_totalElements->nElements(); i++) {
            sTotalElements::sME_Element sElem = sEM_totalElements->getById(i);
            for (unsigned long j: sElem.sMEE_iCElem)
                sEM_gBasicPrices[i] += sEM_gBasicPrices[j];
        }

        sEM_gPastPrices = std::vector<uint32_t>(sEM_gBasicPrices.begin(), sEM_gBasicPrices.end());
        sEM_buyFrequency = {50,200,400,150,500,150};

        sEM_vLastTransactions = std::vector<sRollingListsEvaluator>(sEM_totalElements->nElements(),
                                                                    sRollingListsEvaluator(5));
        sEM_companyHasItem = std::vector<std::list<std::shared_ptr<objCompany>>>(sEM_totalElements->nElements(),
                                                                                 std::list<std::shared_ptr<objCompany>>());
        sEM_productTimer = std::make_shared<sProductTimer>();
        for (int i = 0; i < sEM_totalElements->nElements(); i++) {
            sEM_eyeLastCheckedValue.emplace_back("Prod_" + getStringByObjId(i) + "-mPrice",
                                                 sEM_gBasicPrices[i]);
        }
    };

    bool someCompletedProducts(const uint32_t inTDate) {
        return sEM_productTimer->hasToChange(inTDate);
    }

    std::vector<std::pair<uint32_t, uint32_t>> getCompletedProducts(const uint32_t inTDate) {
        return sEM_productTimer->checkForTime(inTDate);
    }

    std::vector<std::pair<std::string, uint64_t>> getVecAllItemsPrices() {
        std::vector<std::pair<std::string, uint64_t>> _retVec;
        for (const uint64_t uuidObj: sEM_totalElements->extractUuids())
            _retVec.emplace_back(getStringByObjId(uuidObj), getPriceItemActual(uuidObj));

        return _retVec;
    }

    [[nodiscard]] std::string getStringByObjId(const uint32_t inIdProduct) const {
        return sEM_totalElements->getNameById(inIdProduct);
    }

    void tickReduced(uint32_t inRTick, uint32_t inTDate) {
        if (inRTick == 0)
            updateStaticElements();
    }

    void computeCreatedElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany) {
        sEM_gAvailableItems[inUuidElement] += 1;
        sEM_vLastTransactions[inUuidElement].addLastCreate();

        diffElementCompany(inUuidElement, 1, inObjCompany);
        sEM_companyHasItem[inUuidElement].push_back(inObjCompany);
    }

    void
    computeBoughtElementCiv(uint64_t inUuidElement, uint32_t inQuantityElement, uint32_t inRTime, uint32_t inCDate) {
        for (int i = 0; i < inQuantityElement; i++) {
            sEM_vLastTransactions[inUuidElement].addLastBought();

            if (sEM_gAvailableItems.count(inUuidElement) && sEM_gAvailableItems[inUuidElement] > 0)
                _takeAndPayObjectLocalComp(inUuidElement, inRTime, inCDate);
        }
    }

    void computeBoughtElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                              uint32_t inCDate) {
        uint32_t _pItem = getPriceItemActual(inUuidElement);
        sEM_vLastTransactions[inUuidElement].addLastBought();

        if (sEM_gAvailableItems.count(inUuidElement) && sEM_gAvailableItems[inUuidElement] > 0)
            _takeAndPayObjectLocalComp(inUuidElement, inRTime, inCDate);
        else
            sEventManager::getInstance()->callEventCompanyBoughtItemImport(inRTime, inCDate, inObjCompany->c_uuid,
                                                                           inUuidElement, _pItem);

        diffElementCompany(inUuidElement, 1, inObjCompany);
        inObjCompany->addPayment((double) _pItem * -1, oPC_TypePayment::oPC_TP_SOLD, inRTime, inCDate);
    }

    void
    computeSellInmElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                          uint32_t inCDate) {
        uint32_t _pItem = getPriceItemActual(inUuidElement);

        diffElementCompany(inUuidElement, -1, inObjCompany);
        inObjCompany->addPayment((double) _pItem, oPC_TypePayment::oPC_TP_SOLD, inRTime, inCDate);
        sEventManager::getInstance()->callEventCompanySoldItemExport(inRTime, inCDate, inObjCompany->c_uuid,
                                                                     inUuidElement, _pItem);
    }

    sTotalElements::sME_Element getById(uint64_t inUuidElement) { return sEM_totalElements->getById(inUuidElement); }

    bool doesObjectExists(const uint32_t uuidItem) {
        return uuidItem < sEM_totalElements->nElements();
    }

    void changeStateEye(const uint32_t uuidItem) {
        if (sEM_eyeLastCheckedValue[uuidItem].isObserved())
            sEM_eyeLastCheckedValue[uuidItem].removeObserver();
        else
            sEM_eyeLastCheckedValue[uuidItem].setObserver(eyeCatcherActive::getInstance());
    }

    bool getStateEye(const uint32_t uuidItem) {
        return sEM_eyeLastCheckedValue[uuidItem].isObserved();
    }

    [[nodiscard]] bool
    canCompanyProduceObject(const std::shared_ptr<objCompany> &inObjCompany, uint64_t inUuidProduct) {
        if (!doesObjectExists(inUuidProduct))
            return false;
        if (!_hasTypeOwn(inObjCompany, getById(inUuidProduct)))
            return false;
        return true;
    }

    std::vector<uint32_t>
    getVecMissingProducts(const std::shared_ptr<objCompany> &inObjCompany, uint64_t inUuidProduct) {
        std::map<uint32_t, uint8_t> gQuant; //TO IMPROVE;
        for (const auto &nObj: getById(inUuidProduct).sMEE_iCElem)
            gQuant[nObj]++;

        std::vector<uint32_t> rOutput;
        for (const auto &elem: getById(inUuidProduct).sMEE_iCElem)
            if (inObjCompany->c_pOwn.count(elem) && inObjCompany->c_pOwn[elem] < gQuant[elem])
                rOutput.insert(rOutput.end(), gQuant[elem] - inObjCompany->c_pOwn[elem], elem);

        return rOutput;
    }

    void consumeMaterialsProduceObject(const std::shared_ptr<objCompany> &inObjCompany, const uint64_t inUuidProduct,
                                       const uint32_t inCDate) {
        for (const auto &gElem: getById(inUuidProduct).sMEE_iCElem)
            inObjCompany->c_pOwn[gElem] -= 1;
        inObjCompany->c_cAvailableByType[getById(inUuidProduct).sMEE_iReqTypeBuild] -= 1;
        sEM_productTimer->addTimer(inUuidProduct,
                                   getById(inUuidProduct).sMEE_iTime + inCDate,
                                   inObjCompany->c_uuid);
    }

    std::vector<uint32_t> getVecConsumeProductsByPopulation(const uint64_t inNPopulation){
        std::vector<uint32_t> _ret  = std::vector<uint32_t>(sEM_gBasicPrices.size());
        for(int i =0; i < sEM_gBasicPrices.size(); i++) {
            //f(x)=((3)/(1+ℯ^(-((x)/(5)))))-0.5
            _ret[i] = (uint32_t)(((double)inNPopulation / sEM_buyFrequency[i]) * std::max(1.0, (3.0 / (1.0 + std::exp(-getDifferencePrice(i) / 5.0))) - 0.5));
        }
        return _ret;
    }

    int getDifferencePrice(const uint32_t uuidProduct){
        return (int)sEM_gPastPrices[uuidProduct] - (int)sEM_gBasicPrices[uuidProduct];
    }

private:

    static bool
    _hasTypeOwn(const std::shared_ptr<objCompany> &oC, const sTotalElements::sME_Element &inItemGen) {
        if (oC->c_cAvailableByType.find(inItemGen.sMEE_iReqTypeBuild) == oC->c_cAvailableByType.end())
            return false;
        return oC->c_cAvailableByType[inItemGen.sMEE_iReqTypeBuild] > 0;
    }

    static bool _hasResources(const objCompany &oC, const sTotalElements::sME_Element &inItemGen) {
        std::map<uint32_t, uint8_t> gQuant; //TO IMPROVE;
        for (const auto &nObj: inItemGen.sMEE_iCElem)
            gQuant[nObj]++;

        return std::all_of(gQuant.begin(), gQuant.end(), [&](const auto &pair) {
            return oC.c_pOwn.find(pair.first) != oC.c_pOwn.end() && oC.c_pOwn.at(pair.first) >= pair.second;
        });
    }

    void _takeAndPayObjectLocalComp(uint64_t inUuidElement, uint32_t inRTime, uint32_t inCDate) {
        std::shared_ptr<sEventManager> sEM = sEventManager::getInstance();

        uint32_t _pItem = getPriceItemActual(inUuidElement);
        diffElementCompany(inUuidElement, -1, sEM_companyHasItem[inUuidElement].front());
        sEM_companyHasItem[inUuidElement].front()->addPayment((double) _pItem, oPC_TypePayment::oPC_TP_SOLD, inRTime,
                                                              inCDate);
        sEM->callEventCompanySoldItemLocal(inRTime, inCDate, sEM_companyHasItem[inUuidElement].front()->c_uuid,
                                           inUuidElement, _pItem);

        sEM_gAvailableItems[inUuidElement] -= 1;
        sEM_companyHasItem[inUuidElement].pop_front();
    }

    static void
    diffElementCompany(uint64_t inUuidElement, int inNElements, const std::shared_ptr<objCompany> &inPObjCompany) {
        if (inPObjCompany != nullptr) {
            if (inPObjCompany->c_pOwn.find(inUuidElement) == inPObjCompany->c_pOwn.end())
                inPObjCompany->c_pOwn[inUuidElement] = inNElements;
            else
                inPObjCompany->c_pOwn[inUuidElement] += inNElements;
        }
    }

    uint32_t getPriceItemActual(uint64_t inUuidElement) {
        uint32_t _objPrice = sEM_totalElements->getById(inUuidElement).getPrice(
                sEM_vLastTransactions[inUuidElement].getDesirability(), sEM_gBasicPrices);

        if (sEM_eyeLastCheckedValue[inUuidElement].get() != _objPrice)
            sEM_eyeLastCheckedValue[inUuidElement].set(_objPrice);

        if (sEM_gAvailableItems[inUuidElement] <= 0)
            return (uint32_t) (1.1 * _objPrice);
        return std::max((uint32_t)1,_objPrice);
    }

    void updateStaticElements() {
        //They are update only for the information that the falled window of rolling proportions us.
        for (int i = 0; i < sEM_gBasicPrices.size(); i++)
            sEM_gBasicPrices[i] = getById(i).getPrice(sEM_vLastTransactions[i].dropLastWindow(), sEM_gBasicPrices);

        if (nDaysPassed >= 7) {
            nDaysPassed = 0;
            sEM_gPastPrices = std::vector<uint32_t>(sEM_gBasicPrices.begin(), sEM_gBasicPrices.end());
        } else
            nDaysPassed++;
    }

    std::shared_ptr<sTotalElements> sEM_totalElements;
    std::shared_ptr<sProductTimer> sEM_productTimer;

    std::vector<uint32_t> sEM_gBasicPrices;
    std::vector<uint32_t> sEM_gPastPrices;
    std::vector<uint32_t> sEM_buyFrequency;
    uint8_t nDaysPassed;

    std::vector<sRollingListsEvaluator> sEM_vLastTransactions;

    std::vector<eyeValue<uint32_t>> sEM_eyeLastCheckedValue;
    std::map<uint64_t, uint32_t> sEM_gAvailableItems;

    std::vector<std::list<std::shared_ptr<objCompany>>> sEM_companyHasItem;

};

#endif //CITYOFWEIRDFISHES_SEVALUATORMAIN_H
