//
// Created by ganymede on 4/21/24.
//

#ifndef CITYOFWEIRDFISHES_RCOMPPAYMENTSVIEWLAYER_H
#define CITYOFWEIRDFISHES_RCOMPPAYMENTSVIEWLAYER_H

#include "../../rIMenu.h"
#include "../../../../sim/groups/groupEconomy/groupEconomy.h"
#include "../../../../sim/behaviour/clock/sClockMain.h"

class rCompPaymentsViewLayer : public rIMenu {
public:
    explicit rCompPaymentsViewLayer(const std::shared_ptr<rIMenu> &mParent,
                                    objCompany& inRefComp)
            : rIMenu(mParent, rIMenu::rRelativePos::pBottomLeft, "d_mCompPaymentsViewLayer"), rPVL_refCompany(inRefComp) {
        rPVL_totalTransactions = rPVL_refCompany.getVecAllTransactions();

        printItemsOnScreen();
    }

    void draw(sf::RenderWindow &rW) override {
        rW.draw(rIM_dInfo, &rIM_tsTex.tsTex);
    }

    void setResponse(int v, uint16_t lID) override {}

    bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) override {
        switch (event.type) {
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    rIM_parentMenu->setResponse(-1, 1);
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2<int> pMouse = sf::Mouse::getPosition(rWindow);
                    int _gButtonPressed = getButtonPressed(rWindow, pMouse);
                    if (_gButtonPressed != -1) {
                        if (_gButtonPressed == 0)
                            rPVL_cCurLayerObjects = std::max(rPVL_cCurLayerObjects - 1, 0);
                        else if (_gButtonPressed == 1)
                            rPVL_cCurLayerObjects = std::min(rPVL_cCurLayerObjects + 1, (int)rPVL_totalTransactions.size() /
                                                                                        rPVL_nObjInLayer);
                        printItemsOnScreen();
                    }
                }
                break;
            default:
                break;
        }
        return false;
    }

    void pressedCell(std::pair<int, int> cPressed, uint32_t inPTime, uint32_t inUTime) override {}

private:

    void printItemsOnScreen() {
        for (int i = 0; i < rPVL_nObjInLayer * 5; i += 5) {
            uint32_t vObjAct = rPVL_cCurLayerObjects * rPVL_nObjInLayer + i / 5;

            if (vObjAct < rPVL_totalTransactions.size()) {
                setText(i, getFloatToString2Decimal((float)rPVL_totalTransactions[vObjAct].opc_qPayment)); // Numb Transaction

                sClockMain::sCM_ClockValues sDateVal = sClockMain::unpackDateInfo(rPVL_totalTransactions[vObjAct].opc_pDate);
                setText(i + 1, (sDateVal.sCM_rVDay < 10 ? "0" : "") + std::to_string(sDateVal.sCM_rVDay)); //Day
                setText(i + 2, (sDateVal.sCM_rVMonth < 10 ? "0" : "") + std::to_string(sDateVal.sCM_rVMonth)); //Month
                setText(i + 4, std::to_string(sDateVal.sCM_rVYear));  //Year

                std::string sUsedType;
                switch (rPVL_totalTransactions[vObjAct].opc_pType) {
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
                setText(i + 3,sUsedType); //Type

            } else {
                setText(i, "");
                setText(i + 1, "--");
                setText(i + 2, "--");
                setText(i + 3, "-----");
                setText(i + 4, "----");
            }
        }
    }

    int rPVL_cCurLayerObjects = 0;
    uint16_t rPVL_nObjInLayer = 5;
    std::vector<objPaymentCompany> rPVL_totalTransactions;
    objCompany& rPVL_refCompany;
};

#endif //CITYOFWEIRDFISHES_RCOMPPAYMENTSVIEWLAYER_H
