//
// Created by ganymede on 4/21/24.
//

#ifndef CITYOFWEIRDFISHES_RCOMPPAYMENTSVIEWLAYER_H
#define CITYOFWEIRDFISHES_RCOMPPAYMENTSVIEWLAYER_H


#include "../../rIScrollableMenu.h"
#include "../../../../sim/groups/groupEconomy/groupEconomy.h"
#include "../../../../sim/behaviour/clock/sClockMain.h"

class rCompPaymentsViewLayer : public rIScrollableMenu<objPaymentCompany> {
public:
    explicit rCompPaymentsViewLayer(const std::shared_ptr<rIMenu> &mParent, objCompany &inRefComp)
            : rIScrollableMenu<objPaymentCompany>(mParent, rIMenu::rRelativePos::pBottomLeft,
                                                  "d_mCompPaymentsViewLayer", 5), rPVL_refCompany(inRefComp) {
        rPVL_totalElements = rPVL_refCompany.getVecAllTransactions();
        printItemsOnScreen();
    }

protected:
    void printItemsOnScreen() override {
        for (int i = 0; i < rPVL_nObjInLayer * 5; i += 5) {
            uint32_t vObjAct = rPVL_cCurLayerObjects * rPVL_nObjInLayer + i / 5;

            if (vObjAct < rPVL_totalElements.size()) {
                setText(i, getFloatToString2Decimal(
                        (float) rPVL_totalElements[vObjAct].opc_qPayment)); // Num Transaction

                sClockMain::sCM_ClockValues sDateVal = sClockMain::unpackDateInfo(
                        rPVL_totalElements[vObjAct].opc_pDate);
                setText(i + 1, (sDateVal.sCM_rVDay < 10 ? "0" : "") + std::to_string(sDateVal.sCM_rVDay)); //Day
                setText(i + 2, (sDateVal.sCM_rVMonth < 10 ? "0" : "") + std::to_string(sDateVal.sCM_rVMonth)); //Month
                setText(i + 4, std::to_string(sDateVal.sCM_rVYear));  //Year

                std::string sUsedType;
                switch (rPVL_totalElements[vObjAct].opc_pType) {
                    case oPC_TP_RECURRENT_YEAR:
                        sUsedType = "R_YR";
                        break;
                    case oPC_TP_RECURRENT_MONTH:
                        sUsedType = "R_MH";
                        break;
                    case oPC_TP_RECURRENT_WEEK:
                        sUsedType = "R_WK";
                        break;
                    case oPC_TP_SOLD:
                        sUsedType = "O_SD";
                        break;
                    case oPC_TP_INVESTMENT_START:
                        sUsedType = "I_St";
                        break;
                    case oPC_TP_CONTRACT_LOAN:
                        sUsedType = "I_Lo";
                        break;
                    case oPC_TP_CONTRACT_ADQUIRE_CELL:
                        sUsedType = "C_Adq";
                        break;
                }
                setText(i + 3, sUsedType); //Type

            } else
                for (int j = 0; j < 5; ++j)
                    setText(i + j, "");

        }
    }


private:
    objCompany &rPVL_refCompany;
};

#endif //CITYOFWEIRDFISHES_RCOMPPAYMENTSVIEWLAYER_H
