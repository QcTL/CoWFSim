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

/**
 * @class sEvaluatorMain
 * @brief This class is the one that you will use to interact with the different products that the simulation can produce
 */
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
        std::vector<uint32_t> _buyFrequency = {50, 200, 400, 150, 500, 150};

        for (int i = 0; i < sEM_totalElements->nElements(); i++) {
            sEM_totalProducts.emplace_back(getStringByObjId(i), _gBasicPrices[i], _buyFrequency[i], getById(i));
        }
    };

    /**
     * @fn  bool someCompletedProducts
     * @param inTDate A valid reduced date of the actual time in the simulator
     * @return  True if an amount greater than 0 timers are set to complete in that date, false otherwise
     */
    bool someCompletedProducts(const uint32_t inTDate) {
        return sEM_productTimer->hasToChange(inTDate);
    }

    /**
      * @fn std::vector<std::pair<uint32_t, uint32_t>> getCompletedProducts
      * @param inTDate  A valid reduced date of the actual time in the simulator
      * @return A list of the companies and the product that have been completed for a given date, and removed from the list
      * of remaining timers
      */
    std::vector<std::pair<uint32_t, uint32_t>> getCompletedProducts(const uint32_t inTDate) {
        return sEM_productTimer->checkForTime(inTDate);
    }

    /**
     * @fn std::vector<std::pair<std::string, uint64_t>> getVecAllItemsPrices
     * @return Returns a vector containing the name and the price that all the product have
     */
    std::vector<std::pair<std::string, uint64_t>> getVecAllItemsPrices() {
        std::vector<std::pair<std::string, uint64_t>> _retVec;
        for (const uint64_t uuidObj: sEM_totalElements->extractUuids())
            _retVec.emplace_back(getStringByObjId(uuidObj), sEM_totalProducts[uuidObj].getPriceActual());
        return _retVec;
    }

    /**
     * @fn std::string getStringByObjId
     * @param inIdProduct The uuid of a valid product you want to collect, < sME_listItems.size
     * @return The name in form of string of that product
     */
    [[nodiscard]] std::string getStringByObjId(const uint32_t inIdProduct) const {
        return sEM_totalElements->getNameById(inIdProduct);
    }

    /**
     * @fn void tickReduced
     * @brief Updates the elements in the class that need changing every 5 tick of the simulation
     * @param inRTime uint representing the reduced time of the day
     * @param inTDate uint representing the reduced date of the simulation
     */
    void tickReduced(uint32_t inRTick, uint32_t inTDate) {
        if (inRTick == 0)
            updateStaticElements();
    }

    /**
     * @fn void computeCreatedElement
     * @brief This function should be called when this product is created by a company
     * @param inObjCompany The pointer company that created the item, has to be a valid company
     * @param inUuidElement The uuid of a valid product, it has to be < sEM_totalProducts.size()
     */
    void computeCreatedElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany) {
        sEM_totalProducts[inUuidElement].computeCreatedElementCOMPANY(inObjCompany);
    }

    /**
     * @fn void computeBoughtElementCiv
     * @brief This function should be called when a product is purchased by civilians
     * @param inRTime The reduced time of the actual simulation < 288
     * @param inTDate The reduced date where this creation happens, it has to be a valid reduced date
     * @param inUuidElement The uuid of a valid product, it has to be < sEM_totalProducts.size()
     * @param inQuantityElement The quantity of elements that the civilians have purchased
     */
    void
    computeBoughtElementCiv(uint64_t inUuidElement, uint32_t inQuantityElement, uint32_t inRTime, uint32_t inCDate) {
        for (int i = 0; i < inQuantityElement; i++) {
            sEM_totalProducts[inUuidElement].computeBoughtElementCIVIL(inRTime, inCDate);
        }
    }

    /**
      * @fn void computeBoughtElement
      * @brief This function should be called everytime that product is purchased by a company
      * @param inObjCompany The company that purchased the product
      * @param inRTime The reduced time of the actual simulation < 288
      * @param inTDate The reduced date where this creation happens, it has to be a valid reduced date
      * @param inUuidElement The uuid of a valid product, it has to be < sEM_totalProducts.size()
      */
    void computeBoughtElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                              uint32_t inCDate) {
        sEM_totalProducts[inUuidElement].computeBoughtElementCOMPANY(inObjCompany, inRTime, inCDate);
    }

    /**
     * @fn void computeSellInmElement
     * @brief This function is called when a company wants to sell this item immediately outside the city
     * @param inObjCompany  A valid company that wants to immediately sell the product
     * @param inRTime The reduced time of the actual simulation < 288
     * @param inCDate The reduced date where this creation happens, it has to be a valid reduced date
      * @param inUuidElement The uuid of a valid product, it has to be < sEM_totalProducts.size()
     */
    void
    computeSellInmElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany, uint32_t inRTime,
                          uint32_t inCDate) {
        sEM_totalProducts[inUuidElement].computeSellInm(inObjCompany, inRTime, inCDate);
    }

    /**
     * @fn  sTotalElements::sME_Element getById
     * @param inUuidElement The uuid of a valid product you want to collect, < sME_listItems.size
     * @return A copy of the object and its attributes referenced by the uuid
     */
    sTotalElements::sME_Element getById(uint64_t inUuidElement) { return sEM_totalElements->getById(inUuidElement); }

    /**
     * @fn bool doesObjectExists
     * @param uuidItem A uuid of the product that you want to prove if exists
     * @return Returns true if its a valid uuid that represents a real product, false otherwise
     */
    bool doesObjectExists(const uint32_t uuidItem) {
        return uuidItem < sEM_totalElements->nElements();
    }

    /**
     * @fn void changeStateEye
     * @brief This function changes the EyeState of any of the product inside of the class
     * @param inUuidElement The uuid of a valid product you want to collect, < sME_listItems.size
     */
    void changeStateEye(const uint32_t inUuidElement) {
        sEM_totalProducts[inUuidElement].changeStateEye();
    }

    /**
     * @fn bool getStateEye
     * @param inUuidElement The uuid of a valid product you want to collect, < sME_listItems.size
     * @return The state of the eye for that given product referenced by the inUuidElement
     */
    bool getStateEye(const uint32_t inUuidElement) {
        return sEM_totalProducts[inUuidElement].getStateEye();
    }

    /**
     * @fn bool canCompanyProduceObject
     * @param inObjCompany  A valid company you want to check if it can produce a given product
     * @param inUuidProduct The uuid of a valid product you want to collect, < sME_listItems.size
     * @return True if the company can produce the product, false otherwise
     */
    [[nodiscard]] bool
    canCompanyProduceObject(const std::shared_ptr<objCompany> &inObjCompany, uint64_t inUuidProduct) {
        if (!doesObjectExists(inUuidProduct))
            return false;
        if (!_hasTypeOwn(inObjCompany, getById(inUuidProduct)))
            return false;
        return true;
    }

    /**
     * @fn std::vector<uint32_t> getVecMissingProducts
     * @param inObjCompany The active company that you want to get the missing products to complete the product,
     * it has to be able to produce it
     * @param inUuidProduct The uuid of a valid product you want to collect, < sME_listItems.size
     * @return The vector of the products in form of their uuid that the company lacks to create that product
     */
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

    /**
     * @fn void consumeMaterialsProduceObject
     * @param inObjCompany A pointer to a valid company that you want to consume the materials to produce for
     * @param inUuidProduct The uuid of a valid product you want to collect, < sME_listItems.size that you want to produce
     * @param inCDate A valid compressed date that represent the current time on the simulation
     */
    void consumeMaterialsProduceObject(const std::shared_ptr<objCompany> &inObjCompany, const uint64_t inUuidProduct,
                                       const uint32_t inCDate) {
        for (const auto &gElem: getById(inUuidProduct).sMEE_iCElem)
            inObjCompany->c_pOwn[gElem] -= 1;
        inObjCompany->c_cAvailableByType[getById(inUuidProduct).sMEE_iReqTypeBuild] -= 1;

        uint32_t newDateProduct = sClockMain::addDaysToDate(inCDate, getById(inUuidProduct).sMEE_iTime);
        sEM_productTimer->addTimer(inUuidProduct, newDateProduct, inObjCompany->c_uuid);
        inObjCompany->addProcessing(newDateProduct, inUuidProduct);
    }

    /**
     * @fn std::vector<uint32_t> getVecConsumeProductsByPopulation
     * @param inNPopulation The number of population that reside in the simulation in that point in time
     * @return A vector of the number of products that will be consumed in that moment, where the index is the uuid of the product
     * that will be consumed
     */
    std::vector<uint32_t> getVecConsumeProductsByPopulation(const uint64_t inNPopulation) {
        std::vector<uint32_t> _ret;
        _ret.reserve(sEM_totalProducts.size());
        for (const auto &product: sEM_totalProducts)
            _ret.push_back(product.getNProductsDesirability(inNPopulation));
        return _ret;
    }

private:

    /**
     * @fn bool _hasTypeOwn
     * @param oC A pointer to a valid company that you want to check if it has that product represented with inItemGen
     * @param inItemGen  The uuid of a valid product you want to check that contains, < sME_listItems.size
     * @return True if the company has that product in the inventory, false otherwise.
     */
    static bool
    _hasTypeOwn(const std::shared_ptr<objCompany> &oC, const sTotalElements::sME_Element &inItemGen) {
        if (oC->c_cAvailableByType.find(inItemGen.sMEE_iReqTypeBuild) == oC->c_cAvailableByType.end())
            return false;
        return oC->c_cAvailableByType[inItemGen.sMEE_iReqTypeBuild] > 0;
    }

    /**
     * @fn void updateStaticElements
     * @brief This function updates the fallingWindows of the product to represent always the correct values
     */
    void updateStaticElements() {
        //They are update only for the information that the felt window of rolling proportions us.
        for (auto &sEM_totalProduct: sEM_totalProducts) {
            if (nDaysPassed >= 7)
                sEM_totalProduct.newValuePast();
            sEM_totalProduct.newValueBase();
        }

        nDaysPassed = (nDaysPassed + 1) % 7;
    }

    std::shared_ptr<sTotalElements> sEM_totalElements;
    std::shared_ptr<sProductTimer> sEM_productTimer;

    std::vector<sObjProduct> sEM_totalProducts;

    uint8_t nDaysPassed;
};

#endif //CITYOFWEIRDFISHES_SEVALUATORMAIN_H
