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
#include "items/sObjProduct.h"

class sEvaluatorMain {
public:
    sEvaluatorMain() {
        sEM_totalElements = std::make_shared<sTotalElements>("FObjProduced.csv");
        sEM_productTimer = std::make_shared<sProductTimer>();

        std::vector<uint32_t> _gBasicPrices = std::vector<uint32_t>(sEM_totalElements->nElements(), 100);
        for (int i = 0; i < sEM_totalElements->nElements(); i++) {
            sTotalElements::sME_Element sElem = sEM_totalElements->getById(i);
            for (unsigned long j: sElem.sMEE_iCElem)
                _gBasicPrices[i] += _gBasicPrices[j];
        }
        std::vector<uint32_t>_buyFrequency = {50, 200, 400, 150, 500, 150};

        for(int i = 0; i < sEM_totalElements->nElements(); i++){
            sEM_totalProducts.emplace_back(getStringByObjId(i), _gBasicPrices[i], _buyFrequency[i], getById(i));
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
            _retVec.emplace_back(getStringByObjId(uuidObj), sEM_totalProducts[uuidObj].getPriceActual());

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
        sEM_totalProducts[inUuidElement].computeCreatedElementCOMPANY(inObjCompany);
    }

    void
    computeBoughtElementCiv(uint64_t inUuidElement, uint32_t inQuantityElement, uint32_t inRTime, uint32_t inCDate) {
        for (int i = 0; i < inQuantityElement; i++) {
            sEM_totalProducts[inUuidElement].computeBoughtElementCIVIL(inRTime, inCDate);
        }
    }

    void computeBoughtElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                              uint32_t inCDate) {
        sEM_totalProducts[inUuidElement].computeBoughtElementCOMPANY(inObjCompany, inRTime, inCDate);
    }

    void
    computeSellInmElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                          uint32_t inCDate) {
        sEM_totalProducts[inUuidElement].computeSellInm(inObjCompany, inRTime, inCDate);
    }

    sTotalElements::sME_Element getById(uint64_t inUuidElement) { return sEM_totalElements->getById(inUuidElement); }

    bool doesObjectExists(const uint32_t uuidItem) {
        return uuidItem < sEM_totalElements->nElements();
    }

    void changeStateEye(const uint32_t uuidItem) {
        sEM_totalProducts[uuidItem].changeStateEye();
    }

    bool getStateEye(const uint32_t uuidItem) {
        return sEM_totalProducts[uuidItem].getStateEye();
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
        inObjCompany->addProcessing(getById(inUuidProduct).sMEE_iTime + inCDate, inUuidProduct);
    }

    std::vector<uint32_t> getVecConsumeProductsByPopulation(const uint64_t inNPopulation) {
        std::vector<uint32_t> _ret;
        _ret.reserve(sEM_totalProducts.size());
        for (const auto& product : sEM_totalProducts)
            _ret.push_back(product.getNProductsDesirability(inNPopulation));
        return _ret;
    }

private:

    static bool
    _hasTypeOwn(const std::shared_ptr<objCompany> &oC, const sTotalElements::sME_Element &inItemGen) {
        if (oC->c_cAvailableByType.find(inItemGen.sMEE_iReqTypeBuild) == oC->c_cAvailableByType.end())
            return false;
        return oC->c_cAvailableByType[inItemGen.sMEE_iReqTypeBuild] > 0;
    }

    void updateStaticElements() {
        //They are update only for the information that the falled window of rolling proportions us.
        for (auto & sEM_totalProduct : sEM_totalProducts) {
            if (nDaysPassed >= 7)
                sEM_totalProduct.newValuePast();
            sEM_totalProduct.newValueBase();
        }

        nDaysPassed = (nDaysPassed+1) % 7;
    }

    std::shared_ptr<sTotalElements> sEM_totalElements;
    std::shared_ptr<sProductTimer> sEM_productTimer;

    std::vector<sObjProduct> sEM_totalProducts;

    uint8_t nDaysPassed;
    std::map<uint64_t, uint32_t> sEM_gAvailableItems;

};

#endif //CITYOFWEIRDFISHES_SEVALUATORMAIN_H
