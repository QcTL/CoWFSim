//
// Created by Laminar on 17/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCONTRACTORMAIN_H
#define CITYOFWEIRDFISHES_SCONTRACTORMAIN_H

#include <cmath>

#include "../../../structure/obj/elements/objCompany.h"
#include "../../market/listings/implementations/sLRentCell.h"
#include "../../market/listings/implementations/sLBuyCell.h"
#include "sContractorTimer.h"
#include "sContractorStorage.h"
#include "../sCompanyStorage.h"

class sContractorMain {
public:
    sContractorMain() = default;

    void addContractToCompany(const std::shared_ptr<objCompany> &inPObjComReceiving,
                              const std::shared_ptr<objCompany> &inPObjComGiving,
                              const sLRentCell::sMOffering &inSMRentOffer, const uint32_t inTDate) {
        uint32_t _cRecu;
        uint32_t _cTotal;
        switch (inSMRentOffer.sMO_typePayment) {
            case LC_PAY_MONTH:
                _cRecu = inSMRentOffer.sMO_totalPrice / 4;
                _cTotal = static_cast<uint32_t>(std::ceil((float) inSMRentOffer.sMO_totalPrice * 1.2f));
                break;
            case LC_PAY_YEAR:
                _cRecu = inSMRentOffer.sMO_totalPrice / 2;
                _cTotal = static_cast<uint32_t>(std::ceil((float) inSMRentOffer.sMO_totalPrice * 1.10f));
                break;
            case LC_PAY_FORTNIGHT:
                _cRecu = inSMRentOffer.sMO_totalPrice / 5;
                _cTotal = static_cast<uint32_t>(std::ceil((float) inSMRentOffer.sMO_totalPrice * 1.25f));
                break;
        }
        std::shared_ptr<con_rentCell> _rentCell = std::make_shared<con_rentCell>(
                con_rentCell(inPObjComReceiving->c_uuid, inPObjComGiving->c_uuid,
                             inTDate, 0, //TODO DATES;
                             inSMRentOffer.sMO_typePayment, _cRecu, _cTotal,
                             {inSMRentOffer.sMO_pos}, inSMRentOffer.sMO_typePos));
        uint32_t _uuidContract = sCM_ContractorSt->addCon(_rentCell);
        _rentCell->addAsReceiving(inPObjComReceiving);
        _rentCell->addAsGiving(inPObjComGiving);
        //TODO ADD AS CONTRACTOR TIMER;
        inPObjComGiving->addContractToCompany(con_Type::con_Type_Rent, _uuidContract);
        inPObjComReceiving->addContractToCompany(con_Type::con_Type_Rent, _uuidContract);
    }

    void addContractToCompany(const std::shared_ptr<objCompany> &inPObjComReceiving,
                              const std::shared_ptr<objCompany> &inPObjComGiving,
                              const sLBuyCell::sMOffering &inSMBuyOffer, const uint32_t inTDate) {
        uint32_t _cRecu;
        uint32_t _cTotal;
        switch (inSMBuyOffer.sMO_typePayment) {
            case LC_PAY_MONTH:
                _cRecu = inSMBuyOffer.sMO_totalPrice / 4;
                _cTotal = static_cast<uint32_t>(std::ceil((float) inSMBuyOffer.sMO_totalPrice * 1.2f));
                break;
            case LC_PAY_YEAR:
                _cRecu = inSMBuyOffer.sMO_totalPrice / 2;
                _cTotal = static_cast<uint32_t>(std::ceil((float) inSMBuyOffer.sMO_totalPrice * 1.10f));
                break;
            case LC_PAY_FORTNIGHT:
                _cRecu = inSMBuyOffer.sMO_totalPrice / 5;
                _cTotal = static_cast<uint32_t>(std::ceil((float) inSMBuyOffer.sMO_totalPrice * 1.25f));
                break;
        }
        std::shared_ptr<con_buyCell> _buyCell
                = std::make_shared<con_buyCell>(con_buyCell(inPObjComReceiving->c_uuid,
                                                            inPObjComGiving->c_uuid, inTDate,
                                                            inSMBuyOffer.sMO_typePayment, _cRecu, _cTotal,
                                                            {inSMBuyOffer.sMO_pos}, inSMBuyOffer.sMO_typePos));
        uint32_t _uuidContract = sCM_ContractorSt->addCon(_buyCell);
        _buyCell->addAsReceiving(inPObjComReceiving);
        _buyCell->addAsGiving(inPObjComGiving);
        //TODO ADD AS CONTRACTOR TIMER;
        inPObjComGiving->addContractToCompany(con_Type::con_Type_Buy, _uuidContract);
        inPObjComReceiving->addContractToCompany(con_Type::con_Type_Buy, _uuidContract);
    }

    void addContractToCompany(const std::shared_ptr<objCompany> &inPObjComGiving, const std::pair<int, int> &inEHome,
                              const uint32_t inESalary, const uint32_t inTDate) {
        std::shared_ptr<con_hireInteraction> _hireE
                = std::make_shared<con_hireInteraction>(
                        con_hireInteraction(inPObjComGiving->c_uuid, inEHome, inTDate, inTDate));
        uint32_t _uuidContract = sCM_ContractorSt->addCon(_hireE);
        _hireE->addAsGiving(inPObjComGiving);

        inPObjComGiving->addContractToCompany(con_Type::con_Type_Rent, _uuidContract);
    }

    void removeContractFromCompany(const uint32_t inCUuid, std::shared_ptr<sCompanyStorage> &inSCS) {
        std::shared_ptr<con_b> _conB = sCM_ContractorSt->getConByUuid(inCUuid);
        _conB->removeAsGiving(inSCS->getCompanyByUUID(_conB->ct_uuidCompanyGiving));
        if (_conB->ct_hasReceiving)
            _conB->removeAsReceiving(inSCS->getCompanyByUUID(_conB->ct_uuidCompanyReceiving));
    }

private:
    std::shared_ptr<sContractorStorage> sCM_ContractorSt;
    std::shared_ptr<sContractorTimer> sCM_ContractorTi;
};

#endif //CITYOFWEIRDFISHES_SCONTRACTORMAIN_H
