//
// Created by ganymede on 4/1/24.
//

#ifndef CITYOFWEIRDFISHES_GROUPECONOMY_H
#define CITYOFWEIRDFISHES_GROUPECONOMY_H

#include <memory>
#include "../../behaviour/market/sMarketBazaar.h"
#include "../../behaviour/market/sEvaluatorMain.h"
#include "../../behaviour/market/listings/sRollingListsRent.h"

class groupEconomy {
public:
    groupEconomy() = default;

    void setEmptySlots(const std::list<sgTerrain::sgT_CellSlot> &inLEmpty) {
        for (const sgTerrain::sgT_CellSlot &sCellSlot: inLEmpty) {
            for (const auto &pElem: sCellSlot.sgT_gPos) {
                std::shared_ptr<sLBuyCell::sMOffering> sMO =
                        std::make_shared<sLBuyCell::sMOffering>(nullptr, pElem,
                                                                sCellSlot.sgT_soilType,
                                                                0, 0); //TODO PRICE AND QUALITY
                addListing<sLBuyCell>(sMO);
            }
        }
    }

    /**
     * @fn void tickReduced
     * @brief Updates the elements in the class that need changing every 5 tick of the simulation
     * @param inRTime uint representing the reduced time of the day
     * @param inTDate uint representing the reduced date of the simulation
     */
    void tickReduced(const uint32_t inRTime, const uint32_t inTDate) {
        gE_sEvaluator->tickReduced(inRTime, inTDate);
    }

    /**
    * @fn    std::shared_ptr<sLBuyCell::sMOffering> getListOfOffering
    * @brief Returns an offering that complies with the filter given as a parameter
    * @param filter A valid filter for T cells
    * @return A pointer to a valid offering that complies with the filter given
    */
    template<typename T>
    std::shared_ptr<typename T::sMOffering> getListOfOffering(typename T::sMFilter &sMFilter) {
        return gE_sMarketBazaar->getListOfOffering(sMFilter);
    }

    /**
     * @fm void removeCompleteProcess
     * @brief Given the Offering of a T cell, that is contained in the listing, remove's it.
     * @param element A valid SmOffering that is in the listing that you are trying to remove it from
     */
    template<typename T>
    void removeCompleteProcess(std::shared_ptr<typename T::sMOffering> &inSMOffer) {
        gE_sMarketBazaar->removeCompleteProcess(inSMOffer);
    }

    /**
     * @fn bool addListing
     * @brief Adds an offer into the listing for T cells in this class
     * @param sMO The offering you want to add into the listing
     * @return returns true if it has been correctly inserted, false otherwise
     */
    template<typename T>
    void addListing(std::shared_ptr<typename T::sMOffering> &inSMOffer) {
        gE_sMarketBazaar->addListing(inSMOffer);
    }

    // FUNCTIONS EVALUATOR
    /**
 * @fn void computeCreatedElement
     * @brief This function should be called when this product is created by a company
     * @param inObjCompany The pointer company that created the item, has to be a valid company
     * @param inUuidElement The uuid of a valid product, it has to be < sEM_totalProducts.size()
     */
    void computeCreatedElement(uint64_t inUuidElement, const std::shared_ptr<objCompany> &inObjCompany) {
        gE_sEvaluator->computeCreatedElement(inUuidElement, inObjCompany);
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
        gE_sEvaluator->computeBoughtElement(inUuidElement, inObjCompany, inRTime, inCDate);
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
        gE_sEvaluator->computeBoughtElementCiv(inUuidElement, inQuantityElement, inRTime, inCDate);
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
        gE_sEvaluator->computeSellInmElement(inUuidElement, inObjCompany, inRTime, inCDate);
    }


    /**
     * @fn bool doesObjectExists
     * @param uuidItem A uuid of the product that you want to prove if exists
     * @return Returns true if its a valid uuid that represents a real product, false otherwise
     */
    bool doesObjectExists(const uint32_t uuidItem) {
        return gE_sEvaluator->doesObjectExists(uuidItem);
    }

    /**
     * @fn std::vector<std::pair<std::string, uint64_t>> getVecAllItemsPrices
     * @return Returns a vector containing the name and the price that all the product have
     */
    [[nodiscard]] std::vector<std::pair<std::string, uint64_t>> getVecAllItemsPrices() const{
        return gE_sEvaluator->getVecAllItemsPrices();
    }

    /**
     * @fn void changeStateEyeProduct
     * @brief This function changes the EyeState of any of the product inside of the class
     * @param inUuidElement The uuid of a valid product you want to collect, < sME_listItems.size
     */
    void changeStateEyeProduct(const uint32_t inUuidProduct){
        gE_sEvaluator->changeStateEye(inUuidProduct);
    }

    /**
     * @fn bool getStateEyeProduct
     * @param inUuidElement The uuid of a valid product you want to collect, < sME_listItems.size
     * @return The state of the eye for that given product referenced by the inUuidElement
     */
    bool getStateEyeProduct(const uint32_t inUuidProduct){
        return gE_sEvaluator->getStateEye(inUuidProduct);
    }

    sTotalElements::sME_Element getById(uint64_t inUuidElement) { return gE_sEvaluator->getById(inUuidElement); }

    std::string getNameById(uint64_t inUuidElement){return gE_sEvaluator->getStringByObjId(inUuidElement);}

    //Parameters
    std::shared_ptr<sRollingListsRent> gE_sRLR = std::make_shared<sRollingListsRent>(10);
    std::shared_ptr<sEvaluatorMain> gE_sEvaluator = std::make_shared<sEvaluatorMain>();
    std::shared_ptr<sMarketBazaar> gE_sMarketBazaar = std::make_shared<sMarketBazaar>();
};

#endif //CITYOFWEIRDFISHES_GROUPECONOMY_H
