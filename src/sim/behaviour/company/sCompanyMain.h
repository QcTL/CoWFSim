//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMPANYMAIN_H
#define CITYOFWEIRDFISHES_SCOMPANYMAIN_H

#include "sCompanyTimer.h"
#include "sCompanyStorage.h"
#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"
#include "employee/sCivilMain.h"
#include "code/sCodeStoratge.h"
#include "../../groups/groupEconomy/groupEconomy.h"
#include "contract/sContractorMain.h"

class sCompanyMain {
public:

    enum sCM_strStyleCompany {
        SCM_strOffice = 0, SCM_strFactory = 1, SCM_strField = 2
    };

    explicit sCompanyMain(uint32_t inGridSize, const std::shared_ptr<sCivilMain> &inPCivilMain,
                          const std::shared_ptr<groupLand> &inGLand, const std::shared_ptr<groupEconomy> &inGEconomy)
            : sM_groupLand(inGLand), sM_sCompEmployee(inPCivilMain), sM_groupEconomy(inGEconomy) {
        sTComp = std::make_shared<sCompanyStorage>(inGridSize, 1000, sSCode);
    }

    void tickReduced(const uint32_t inRTime, const uint32_t inTDate) {
        if (sM_sCompanyTimer->hasToChange(inTDate))
            for (std::pair<uint32_t, uint32_t> &t: sM_sCompanyTimer->checkForTime(inTDate))
                FulfillIntentions::gCompletedProduct(sTComp->getCompanyByUUID(t.second), t.first, sM_groupEconomy);

        if (inRTime % (12 * 3) == 0) { //ONCE EVERY 3 HOURS
            for (const sCompanyCompiler::sCCIntentions &sCCI: sTComp->getTotalIntentions(inRTime)) {
                FulfillIntentions::gTryIntention(sCCI, *this, inTDate);
                sSCode->updateScoreCode(sCCI.scc_objCompany->c_uuid, -1);
            }
        }

        std::vector<std::pair<uint32_t, int>> sDiffEmp = sTComp->getDiffEmployeesByLocation(inRTime);
        for (const std::pair<uint32_t, int> &dEmp: sDiffEmp)
            if (dEmp.second > 0)
                for (int i = 0; i < dEmp.second; i++)
                    FulfillIntentions::gTryIntention({sCompanyCompiler::sCCIntentions::GEN_HireEmployee, 0,
                                                      sTComp->getCompanyByUUID(dEmp.first)}, *this, inTDate);
            else
                for (int i = 0; i < abs(dEmp.second); i++)
                    FulfillIntentions::gTryIntention({sCompanyCompiler::sCCIntentions::GEN_TerminateEmployee, 0,
                                                      sTComp->getCompanyByUUID(dEmp.first)}, *this, inTDate);

        if (inRTime == 0) {
            if ((inTDate % 1 << 9) == 0)
                sTComp->applyEcoYear();
            if ((inTDate % 1 << 5) == 0)
                sTComp->applyEcoMonth();
            if ((inTDate % 1 << 3) == 0)
                sTComp->applyEcoWeek();
        }

        //Start bankruptcy
        std::vector<std::shared_ptr<objCompany>> vCompBankruptcy = sTComp->getVecCompBankruptcy(inRTime);
        for (const std::shared_ptr<objCompany> &oCompany: vCompBankruptcy)
            for (int i = 0; i < oCompany->c_cActiveLocations.size(); i++)
                FulfillIntentions::gTryIntention({sCompanyCompiler::sCCIntentions::CELL_Sell, (uint32_t) i,
                                                  oCompany}, *this, inTDate);
    }

    void completedStartCompanies(const std::list<sgTerrain::sgT_emptySlot> &gPosCompanies) {
        for (const auto &posNewComp: gPosCompanies) {
            uint32_t uuidNew = sTComp->createCompany({posNewComp.sgT_gPos},
                                                     posNewComp.sgT_gType);
            for (int i = 0; i < 2; i++)
                FulfillIntentions::gTryIntention({sCompanyCompiler::sCCIntentions::GEN_HireEmployee, 0,
                                                  sTComp->getCompanyByUUID(uuidNew)}, *this, 0); //TODO DATE;
            //TODO, hauria de ser similar al addNewCompany, però el fet que ja tinguin les caselles ja seleccionades és una merda.
        }
    }

    void addNewCompany(sCM_strStyleCompany cCompanyCreation) {
        FulfillIntentions::gTryIntention(
                {sCompanyCompiler::sCCIntentions::GEN_CreateCompany, cCompanyCreation, nullptr}, *this, 0);
    }

    //STORAGE

    std::shared_ptr<sCodeStorage> sSCode = std::make_shared<sCodeStorage>();
    std::shared_ptr<sCompanyStorage> sTComp;
private:
    std::shared_ptr<groupLand> sM_groupLand;
    std::shared_ptr<groupEconomy> sM_groupEconomy;

    std::shared_ptr<sCompanyTimer> sM_sCompanyTimer = std::make_shared<sCompanyTimer>();
    std::shared_ptr<sCivilMain> sM_sCompEmployee;
    std::shared_ptr<sContractorMain> sM_sContractor;

    class FulfillIntentions {
    public:
        static bool gTryIntention(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                  const uint32_t cDate) {
            switch (sCCI.scc_type) {
                case sCompanyCompiler::sCCIntentions::CELL_Buy:
                    gExecuteBuyCellOperation(sCCI, inSCM, cDate);
                    break;
                case sCompanyCompiler::sCCIntentions::CELL_GiveRent:
                    gExecuteGiveRentCellOperation(sCCI, inSCM, cDate);
                    break;
                case sCompanyCompiler::sCCIntentions::CELL_GainRent:
                    gExecuteGainRentCellOperation(sCCI, inSCM, cDate);
                    break;
                case sCompanyCompiler::sCCIntentions::CELL_Sell:
                    gExecuteSellCellOperation(sCCI, inSCM, cDate);
                    break;
                case sCompanyCompiler::sCCIntentions::OBJ_Produce:
                    gExecuteProduceObjOperation(sCCI, inSCM, cDate);
                    break;
                case sCompanyCompiler::sCCIntentions::OBJ_Buy:
                    gExecuteBuyObjOperation(sCCI, inSCM, cDate);
                    break;
                case sCompanyCompiler::sCCIntentions::OBJ_SellInm:
                    gExecuteSellObjOperation(sCCI, inSCM, cDate);
                    break;
                case sCompanyCompiler::sCCIntentions::GEN_CreateCompany:
                    gExecuteCreateCompanyGenOperation(sCCI, inSCM, cDate);
                    break;
                case sCompanyCompiler::sCCIntentions::GEN_HireEmployee:
                    gExecuteHireEmployeeGenOperation(sCCI, inSCM, cDate);
                    break;
                case sCompanyCompiler::sCCIntentions::GEN_TerminateEmployee:
                    gExecuteTerminateEmployeeGenOperation(sCCI, inSCM, cDate);
                    break;
            }
            return true;
        }

        static void gCompletedProduct(const std::shared_ptr<objCompany> &oC, uint32_t gItemGen,
                                      std::shared_ptr<groupEconomy> &inGEconomy) {
            inGEconomy->computeCreatedElement(gItemGen, oC);
            oC->c_cAvailableByType[inGEconomy->getById(gItemGen).sMEE_iReqTypeBuild] += 1;
        }

    private:
        // BUY CELL
        static void
        gExecuteBuyCellOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                 const uint32_t cDate) {
            sLBuyCell::sMFilter sMF(sCCI.scc_addIdInfo);
            std::shared_ptr<sLBuyCell::sMOffering> sMOff = inSCM.sM_groupEconomy->getListOfOffering<sLBuyCell>(sMF);
            if (sMOff != nullptr) {
                inSCM.sM_sContractor->addContractToCompany(sCCI.scc_objCompany, sMOff->sMO_givingCompany,
                                                           *sMOff, cDate);
                inSCM.sTComp->addRefPosOwnCompany(sMOff->sMO_pos, sCCI.scc_objCompany->c_uuid);
                inSCM.sM_groupLand->gL_gTerrain->addNewBuilding(sMOff->sMO_pos);
                inSCM.sM_groupEconomy->removeCompleteProcess<sLBuyCell>(sMOff);
            }
        }

        //GIVE RENT CELL
        static void
        gExecuteGiveRentCellOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                      const uint32_t cDate) {
            std::pair<int, int> gTile = sCCI.scc_objCompany->getOwnedByIndex(sCCI.scc_addIdInfo);
            std::shared_ptr<sLRentCell::sMOffering> sMO =
                    std::make_shared<sLRentCell::sMOffering>(sCCI.scc_objCompany, gTile,
                                                             inSCM.sM_groupLand->gL_gTerrain->gTG_TypeGen->get(gTile),
                                                             (uint16_t) 0, (uint32_t) 0,
                                                             con_TypePaymentFreq::LC_PAY_MONTH);
            inSCM.sM_groupEconomy->addListing<sLRentCell>(sMO);
        }

        //GAIN RENT CELL
        static void
        gExecuteGainRentCellOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                      const uint32_t cDate) {
            sLRentCell::sMFilter sMF(sCCI.scc_addIdInfo);
            std::shared_ptr<sLRentCell::sMOffering> sMOff = inSCM.sM_groupEconomy->getListOfOffering<sLRentCell>(sMF);

            inSCM.sM_sContractor->addContractToCompany(sCCI.scc_objCompany, sMOff->sMO_givingCompany, *sMOff, cDate);
            inSCM.sM_groupEconomy->removeCompleteProcess<sLRentCell>(sMOff);
        }

        //SELL CELL
        static void
        gExecuteSellCellOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                  const uint32_t cDate) {
            std::pair<int, int> gTile = sCCI.scc_objCompany->getOwnedByIndex(sCCI.scc_addIdInfo);
            std::shared_ptr<sLBuyCell::sMOffering> sMO =
                    std::make_shared<sLBuyCell::sMOffering>(sCCI.scc_objCompany, gTile,
                                                            inSCM.sM_groupLand->gL_gTerrain->gTG_TypeGen->get(gTile),
                                                            0, 0); //TODO SET QUALITY AND TOTAL PRICE
            inSCM.sM_groupEconomy->addListing<sLBuyCell>(sMO);
            inSCM.sM_groupLand->gL_gTerrain->removeBuilding(gTile);
            inSCM.sTComp->removeRefPosOwnCompany(gTile, sCCI.scc_objCompany->c_uuid);
        }

        //PRODUCE OBJ
        static void
        gExecuteProduceObjOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                    const uint32_t cDate) {
            if (!_gProduceProduct(sCCI, inSCM, cDate))
                inSCM.sSCode->updateScoreCode(sCCI.scc_objCompany->c_uuid, -10);
        }

        static bool
        _gProduceProduct(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM, const uint32_t cDate) {
            if (!_hasTypeOwn(sCCI.scc_objCompany, inSCM.sM_groupEconomy->getById(sCCI.scc_addIdInfo)))
                return false;
            if (!_hasResources(*sCCI.scc_objCompany, inSCM.sM_groupEconomy->getById(sCCI.scc_addIdInfo)))
                for (uint32_t mLack: inSCM.sM_groupEconomy->getById(sCCI.scc_addIdInfo).sMEE_iCElem)
                    gTryIntention({sCompanyCompiler::sCCIntentions::OBJ_Buy, mLack, sCCI.scc_objCompany}, inSCM, cDate);

            for (const auto &gElem: inSCM.sM_groupEconomy->getById(sCCI.scc_addIdInfo).sMEE_iCElem)
                sCCI.scc_objCompany->c_pOwn[gElem] -= 1;
            sCCI.scc_objCompany->c_cAvailableByType[inSCM.sM_groupEconomy->getById(
                    sCCI.scc_addIdInfo).sMEE_iReqTypeBuild] -= 1;
            inSCM.sM_sCompanyTimer->addTimer(sCCI.scc_addIdInfo,
                                             inSCM.sM_groupEconomy->getById(sCCI.scc_addIdInfo).sMEE_iTime + cDate,
                                             sCCI.scc_objCompany->c_uuid);
            std::cout << "CREATED PRODUCT" << std::endl;
            return true;
        }

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

        //BUY OBJ
        static void
        gExecuteBuyObjOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                const uint32_t cDate) {
            inSCM.sM_groupEconomy->computeBoughtElement(sCCI.scc_addIdInfo, sCCI.scc_objCompany);
        }

        //SELL OBJ
        static void
        gExecuteSellObjOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                 const uint32_t cDate) {
            inSCM.sM_groupEconomy->computeSellInmElement(sCCI.scc_addIdInfo, sCCI.scc_objCompany);
        }

        //CREATE COMPANY GEN
        static void
        gExecuteCreateCompanyGenOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                          const uint32_t cDate) {
            std::vector<sgTerrain::sgT_TypeGen> gTypeGivenTC = {sgTerrain::sgT_TypeGen::sgT_TG_CivBuilding,
                                                                sgTerrain::sgT_TypeGen::sgT_TG_IndBuilding,
                                                                sgTerrain::sgT_TypeGen::sgT_TG_FieldBuilding};
            sgTerrain::sgT_TypeGen gTypeCompany = gTypeGivenTC[sCCI.scc_addIdInfo % gTypeGivenTC.size()];
            uint32_t uuidNew = inSCM.sTComp->createCompany({}, gTypeCompany);
            gTryIntention({sCompanyCompiler::sCCIntentions::CELL_Buy, gTypeCompany,
                           inSCM.sTComp->getCompanyByUUID(uuidNew)}, inSCM, cDate);
            for (int i = 0; i < 2; i++) //TODO, make it right
                FulfillIntentions::gTryIntention({sCompanyCompiler::sCCIntentions::GEN_HireEmployee, 0,
                                                  sCCI.scc_objCompany}, inSCM, cDate);
        }

        static void
        gExecuteHireEmployeeGenOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                         const uint32_t cDate) {
            std::pair<int, int> pHouseEmployee = inSCM.sM_sCompEmployee->setRouteToNewEmployee(*sCCI.scc_objCompany);
            inSCM.sM_sContractor->addContractToCompany(sCCI.scc_objCompany, pHouseEmployee,
                                                       400, cDate);

            if (!inSCM.sTComp->isCompanyInPosition(pHouseEmployee))
                sTComp->getCompanyByUUID(gLayerOwnership->get(pHouseEmployee).front())->c_objMonth += 150; //RENT
            //TODO, UN ALTRE CONTRACTE DE RENT DE CIVILIAN QUE SE LI APLICARA A LA EMPRESA A LA QUAL ESTA FENT RENTING, ME CAGO EN TOT, TIOOOOOOO, MAAAAAAAAAAAAAAAAAN
        }

        static void
        gExecuteTerminateEmployeeGenOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                              const uint32_t cDate) {
            std::pair<int, int> pHouseEmployee = inSCM.sM_sCompEmployee->removeEmployeeToCompany(*sCCI.scc_objCompany);
            uint32_t _uuidContractTerminate = sCCI.scc_objCompany->c_activeContracts[con_Type::con_Type_Hire].front();
            sCCI.scc_objCompany->c_activeContracts[con_Type::con_Type_Hire].pop_front();
            inSCM.sM_sContractor->removeContractFromCompany(_uuidContractTerminate, inSCM.sTComp);

            //TODO Remove RENT
            //TODO, UN ALTRE CONTRACTE DE RENT DE CIVILIAN QUE SE LI APLICARA A LA EMPRESA A LA QUAL ESTA FENT RENTING, ME CAGO EN TOT, TIOOOOOOO, MAAAAAAAAAAAAAAAAAN
        }
    };
};

#endif //CITYOFWEIRDFISHES_SCOMPANYMAIN_H