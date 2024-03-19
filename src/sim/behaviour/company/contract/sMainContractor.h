//
// Created by Laminar on 17/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SMAINCONTRACTOR_H
#define CITYOFWEIRDFISHES_SMAINCONTRACTOR_H

#include "../../../structure/obj/elements/objCompany.h"
#include "../../market/listings/implementations/sLRentCell.h"
#include "../../market/listings/implementations/sLBuyCell.h"
#include "sContractorTimer.h"
#include "sContractorStorage.h"

class sMainContractor {
public:
    sMainContractor(){

    }

    void addContractToCompany(std::shared_ptr<objCompany> &objComReceiving,
                              std::shared_ptr<objCompany> &objComGiving,
                              const sLRentCell::sMOffering &sMRentOffer, const uint32_t actTime) {
        uint32_t cRecu;
        uint32_t cTotal;
        switch (sMRentOffer.sMO_typePayment) {
            case LC_PAY_MONTH:
                cRecu = sMRentOffer.sMO_totalPrice / 4;
                cTotal = static_cast<uint32_t>(sMRentOffer.sMO_totalPrice * 1.2f);
                break;
            case LC_PAY_YEAR:
                cRecu = sMRentOffer.sMO_totalPrice / 2;
                cTotal = static_cast<uint32_t>(sMRentOffer.sMO_totalPrice * 1.10f);
                break;
            case LC_PAY_FORTNIGHT:
                cRecu = sMRentOffer.sMO_totalPrice / 5;
                cTotal = static_cast<uint32_t>(sMRentOffer.sMO_totalPrice * 1.25f);
                break;
        }
        std::shared_ptr<con_rentCell> rentCell = std::make_shared<con_rentCell>(
                con_rentCell(objComReceiving->c_uuid, objComGiving->c_uuid,
                             actTime, 0, //TODO DATES;
                             sMRentOffer.sMO_typePayment, cRecu, cTotal, {sMRentOffer.sMO_pos}));
        sStorageCon->addCon(rentCell);
        objComReceiving->addAsReceiving(*rentCell);
        objComGiving->addAsGiving(*rentCell);
        //ADD AS CONTRACTOR TIMER;
    }

    void addContractToCompany(std::shared_ptr<objCompany> &objComReceiving,
                              std::shared_ptr<objCompany> &objComGiving,
                              const sLBuyCell::sMOffering &sMBuyOffer, const uint32_t actTime) {
        uint32_t cRecu;
        uint32_t cTotal;
        switch (sMBuyOffer.sMO_typePayment) {
            case LC_PAY_MONTH:
                cRecu = sMBuyOffer.sMO_totalPrice / 4;
                cTotal = static_cast<uint32_t>(sMBuyOffer.sMO_totalPrice * 1.2f);
                break;
            case LC_PAY_YEAR:
                cRecu = sMBuyOffer.sMO_totalPrice / 2;
                cTotal = static_cast<uint32_t>(sMBuyOffer.sMO_totalPrice * 1.10f);
                break;
            case LC_PAY_FORTNIGHT:
                cRecu = sMBuyOffer.sMO_totalPrice / 5;
                cTotal = static_cast<uint32_t>(sMBuyOffer.sMO_totalPrice * 1.25f);
                break;
        }
        con_buyCell buyCell = {objComReceiving->c_uuid, objComGiving->c_uuid, actTime,
                               sMBuyOffer.sMO_typePayment, cRecu, cTotal, {sMBuyOffer.sMO_pos}};
        objComReceiving->addAsReceiving(buyCell);
        objComGiving->addAsGiving(buyCell);
        //ADD AS CONTRACTOR TIMER;
    }

private:
    std::shared_ptr<sContractorStorage> sStorageCon;
    std::shared_ptr<sContractorTimer> sConT;
};

#endif //CITYOFWEIRDFISHES_SMAINCONTRACTOR_H
