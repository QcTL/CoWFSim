//
// Created by Laminar on 17/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SLBUYCELL_H
#define CITYOFWEIRDFISHES_SLBUYCELL_H


#include "../sIListing.h"
#include "../../../../structure/grids/gIGrid.h"
#include "../../../../structure/grids/gBasicGrid.h"

class sLBuyCell : public sIListing {
public:
    class sMOffering : public sIListing::sMOffering {
    public:
        sMOffering(const std::shared_ptr<objCompany> &givingCompany, const std::pair<int, int> &pos, uint16_t quality,
                   uint32_t totalPrice)
                : sMO_pos(pos), sMO_quality(quality), sMO_totalPrice(totalPrice), sMO_givingCompany(givingCompany) {}

        bool operator==(const sIListing::sMOffering &other) const override {
            const auto *otherOffering = dynamic_cast<const sLBuyCell::sMOffering *>(&other);
            if (!otherOffering)
                return false;
            return sMO_pos == otherOffering->sMO_pos;
        }

        std::pair<int, int> sMO_pos;
        uint16_t sMO_quality;
        uint32_t sMO_totalPrice;
        std::shared_ptr<objCompany> sMO_givingCompany;
        con_TypePaymentFreq sMO_typePayment;
    };

    class sMFilter : public sIListing::sMFilter {
    public:
        explicit sMFilter(uint8_t typeBuild, const std::shared_ptr<gIGrid<uint8_t>> &gTypeGen)
                : typeBuild(typeBuild), gTypeGen(gTypeGen) {}

        [[nodiscard]] bool doesComply(const sIListing::sMOffering &offer) const override {
            const auto *tOffer = dynamic_cast<const sLBuyCell::sMOffering *>(&offer);
            if (!tOffer)
                return false;
            return typeBuild == gTypeGen->get(tOffer->sMO_pos);
        }

    private:
        uint8_t typeBuild;
        std::shared_ptr<gIGrid<uint8_t>> gTypeGen;
    };
};

#endif //CITYOFWEIRDFISHES_SLBUYCELL_H
