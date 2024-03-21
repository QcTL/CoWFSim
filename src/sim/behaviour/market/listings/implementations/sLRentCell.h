//
// Created by Laminar on 17/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SLRENTCELL_H
#define CITYOFWEIRDFISHES_SLRENTCELL_H


#include "../sIListing.h"
#include "../../../../structure/grids/gIGrid.h"
#include "../../../../structure/grids/gBasicGrid.h"
#include "../../../../structure/obj/elements/contracts/cObjContracts.h"

class sLRentCell : public sIListing {
public:
    class sMOffering : public sIListing::sMOffering {
    public:
        sMOffering(const std::shared_ptr<objCompany> &givingCompany, const std::pair<int, int> &pos, uint8_t typePos,
                   uint16_t quality,
                   uint32_t totalPrice, con_TypePaymentFreq typePayment)
                : sMO_pos(pos), sMO_quality(quality), sMO_totalPrice(totalPrice), sMO_typePos(typePos),
                  sMO_typePayment(typePayment), sMO_givingCompany(givingCompany) {}

        bool operator==(const sIListing::sMOffering &other) const override {
            const auto *otherOffering = dynamic_cast<const sLRentCell::sMOffering *>(&other);
            if (!otherOffering)
                return false;
            return sMO_pos == otherOffering->sMO_pos;
        }

        std::pair<int, int> sMO_pos;
        uint8_t sMO_typePos;
        uint16_t sMO_quality;
        uint32_t sMO_totalPrice;
        std::shared_ptr<objCompany> sMO_givingCompany;
        con_TypePaymentFreq sMO_typePayment;
    };

    class sMFilter : public sIListing::sMFilter {
    public:
        explicit sMFilter(uint8_t typeBuild)
                : typeBuild(typeBuild) {}

        [[nodiscard]] bool doesComply(const sIListing::sMOffering &offer) const override {
            const auto *tOffer = dynamic_cast<const sLRentCell::sMOffering *>(&offer);
            if (!tOffer)
                return false;
            return typeBuild == tOffer->sMO_typePos;
        }

    private:
        uint8_t typeBuild;
    };
};

#endif //CITYOFWEIRDFISHES_SLRENTCELL_H
