#pragma clang diagnostic push
#pragma ide diagnostic ignored "ArgumentSelectionDefects"
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
#include "../globalAttr/stGlobalTrackerAttr.h"

class sCompanyMain {
public:

    enum sCM_strStyleCompany {
        SCM_strOffice = 0, SCM_strFactory = 1, SCM_strField = 2
    };

    explicit sCompanyMain(uint32_t inGridSize, const std::shared_ptr<sCivilMain> &inPCivilMain,
                          const std::shared_ptr<groupLand> &inGLand, const std::shared_ptr<groupEconomy> &inGEconomy)
            : sM_groupLand(inGLand), sM_sCompEmployee(inPCivilMain), sM_groupEconomy(inGEconomy) {
        sTComp = std::make_shared<sCompanyStorage>(inGridSize, 1000, sSCode);
        sM_sContractor = std::make_shared<sContractorMain>();
    }

    void tickReduced(const uint32_t inRTime, const uint32_t inTDate) {
        if (sM_sCompanyTimer->hasToChange(inTDate))
            for (std::pair<uint32_t, uint32_t> &t: sM_sCompanyTimer->checkForTime(inTDate))
                FulfillIntentions::gCompletedProduct(sTComp->getCompanyByUUID(t.second), t.first, sM_groupEconomy);

        if (inRTime % (12 * 3) == 0) { //ONCE EVERY 3 HOURS
            for (const sCompanyCompiler::sCCIntentions &sCCI: sTComp->getTotalIntentions(inRTime)) {
                FulfillIntentions::gTryIntention(sCCI, *this, inRTime, inTDate);
                sSCode->updateScoreCode(sCCI.scc_objCompany->c_uuid, -1);
            }
        }

        std::vector<std::pair<uint32_t, int>> sDiffEmp = sTComp->getDiffEmployeesByLocation(inRTime);
        for (const std::pair<uint32_t, int> &dEmp: sDiffEmp)
            if (dEmp.second > 0)
                for (int i = 0; i < dEmp.second; i++)
                    FulfillIntentions::gTryIntention({sCompanyCompiler::sCCIntentions::GEN_HireEmployee, 0,
                                                      sTComp->getCompanyByUUID(dEmp.first)}, *this, inRTime, inTDate);
            else
                for (int i = 0; i < abs(dEmp.second); i++)
                    FulfillIntentions::gTryIntention({sCompanyCompiler::sCCIntentions::GEN_TerminateEmployee, 0,
                                                      sTComp->getCompanyByUUID(dEmp.first)}, *this, inRTime, inTDate);

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
                                                  oCompany}, *this, 0, inTDate);
    }

    void completedStartCompanies(const std::list<sgTerrain::sgT_CellSlot> &gPosCompanies) {
        for (const auto &posNewComp: gPosCompanies) {
            uint32_t uuidNew = sTComp->createCompany({posNewComp.sgT_gPos}, posNewComp.sgT_gType);
            auto typeSoil = sM_groupLand->gL_gTerrain->gTG_TypeSoil->get(posNewComp.sgT_gPos);
            if (typeSoil != sgTerrain::sgT_TypeSoil::sgT_TS_T2Mixed &&
                typeSoil != sgTerrain::sgT_TypeSoil::sgT_TS_T3Mixed) {
                for (int i = 0; i < 2; i++)
                    FulfillIntentions::gTryIntention({sCompanyCompiler::sCCIntentions::GEN_HireEmployee, 0,
                                                      sTComp->getCompanyByUUID(uuidNew)}, *this, 0, 0); //TODO DATE;
            } else
                sTComp->getCompanyByUUID(uuidNew)->c_attrCanEmployee = false;
            FulfillIntentions::doRentContractsExistingResidents(posNewComp.sgT_gPos, *this, 0);
        }
    }

    void addNewCompany(sCM_strStyleCompany cCompanyCreation, const uint32_t inRTime, const uint32_t inTDate) {
        FulfillIntentions::gTryIntention(
                {sCompanyCompiler::sCCIntentions::GEN_CreateCompany, cCompanyCreation, nullptr}, *this, +
                        inRTime, inTDate);
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
                                  const uint32_t inRTime, const uint32_t inCDate) {
            switch (sCCI.scc_type) {
                case sCompanyCompiler::sCCIntentions::CELL_Buy:
                    gExecuteBuyCellOperation(sCCI, inSCM, inRTime, inCDate);
                    break;
                case sCompanyCompiler::sCCIntentions::CELL_GiveRent:
                    gExecuteGiveRentCellOperation(sCCI, inSCM, inRTime, inCDate);
                    break;
                case sCompanyCompiler::sCCIntentions::CELL_GainRent:
                    gExecuteGainRentCellOperation(sCCI, inSCM, inRTime, inCDate);
                    break;
                case sCompanyCompiler::sCCIntentions::CELL_Sell:
                    gExecuteSellCellOperation(sCCI, inSCM, inRTime, inCDate);
                    break;
                case sCompanyCompiler::sCCIntentions::OBJ_Produce:
                    gExecuteProduceObjOperation(sCCI, inSCM, inRTime, inCDate);
                    break;
                case sCompanyCompiler::sCCIntentions::OBJ_Buy:
                    gExecuteBuyObjOperation(sCCI, inSCM, inRTime, inCDate);
                    break;
                case sCompanyCompiler::sCCIntentions::OBJ_SellInm:
                    gExecuteSellObjOperation(sCCI, inSCM, inRTime, inCDate);
                    break;
                case sCompanyCompiler::sCCIntentions::GEN_CreateCompany:
                    gExecuteCreateCompanyGenOperation(sCCI, inSCM, inRTime, inCDate);
                    break;
                case sCompanyCompiler::sCCIntentions::GEN_HireEmployee:
                    gExecuteHireEmployeeGenOperation(sCCI, inSCM, inRTime, inCDate);
                    break;
                case sCompanyCompiler::sCCIntentions::GEN_TerminateEmployee:
                    gExecuteTerminateEmployeeGenOperation(sCCI, inSCM, inRTime, inCDate);
                    break;
            }
            return true;
        }

        static void gCompletedProduct(const std::shared_ptr<objCompany> &oC, uint32_t gItemGen,
                                      std::shared_ptr<groupEconomy> &inGEconomy) {
            inGEconomy->computeCreatedElement(gItemGen, oC);
            oC->c_cAvailableByType[inGEconomy->getById(gItemGen).sMEE_iReqTypeBuild] += 1;
        }

        static void
        doRentContractsExistingResidents(const std::pair<int, int> &inPHouse, sCompanyMain &inSCM, uint32_t inTDate) {
            uint8_t nResidents = inSCM.sM_groupLand->gL_gTerrain->gTG_civilOccupancy->get(inPHouse);
            if (nResidents <= 0)
                return;
            for (int i = 0; i < nResidents; i++) {
                _addContractRentEmployee(inPHouse, inSCM, inTDate);
            }
        }

    private:
        static uint32_t getTotalPriceOfHome(const std::pair<int, int> &cPos, sCompanyMain &inSCM) {
            uint32_t pValueHome = inSCM.sM_sCompEmployee->getPriceByHouse(cPos);
            return pValueHome - inSCM.sM_groupLand->gL_gAirPollution->getPenalizationAir(cPos);
        }

        // BUY CELL
        static void
        gExecuteBuyCellOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                 const uint32_t inRTime, const uint32_t inCDate) {
            sLBuyCell::sMFilter sMF(sCCI.scc_addIdInfo);
            std::shared_ptr<sLBuyCell::sMOffering> sMOff = inSCM.sM_groupEconomy->getListOfOffering<sLBuyCell>(sMF);
            if (sMOff != nullptr) {
                uint32_t uuidContract = inSCM.sM_sContractor->addContractToCompany(sCCI.scc_objCompany,
                                                                                   sMOff->sMO_givingCompany,
                                                                                   *sMOff, inCDate);
                inSCM.sTComp->addRefPosOwnCompany(sMOff->sMO_pos, sCCI.scc_objCompany->c_uuid);
                inSCM.sM_groupLand->gL_gTerrain->addNewBuilding(sMOff->sMO_pos);
                inSCM.sM_groupEconomy->removeCompleteProcess<sLBuyCell>(sMOff);

                FulfillIntentions::doRentContractsExistingResidents(sMOff->sMO_pos, inSCM, inCDate);


                std::shared_ptr<sEventManager> sEM = sEventManager::getInstance();
                sEM->callEventCompanyBoughtCell_PGiving(inRTime, inCDate,
                                                        sMOff->sMO_givingCompany->c_uuid, uuidContract);
                sEM->callEventCompanyBoughtCell_PReceiving(inRTime, inCDate,
                                                           sCCI.scc_objCompany->c_uuid, uuidContract);
            }
        }

        //GIVE RENT CELL
        static void
        gExecuteGiveRentCellOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                      const uint32_t inRTime, const uint32_t inCDate) {
            std::pair<int, int> gTile = sCCI.scc_objCompany->getOwnedByIndex(sCCI.scc_addIdInfo);
            uint32_t cQuality = inSCM.sM_groupLand->gL_gTerrain->getQualityGivenPosHome(gTile) -
                                inSCM.sM_groupLand->gL_gAirPollution->getPenalizationAir(gTile);

            uint32_t endRentPrice = getTotalPriceOfHome(gTile, inSCM);
            uint32_t cPrice = inSCM.sM_groupLand->gL_gTerrain->gTG_civilOccupancy->get(gTile) * endRentPrice * 1.5;

            std::shared_ptr<sLRentCell::sMOffering> sMO =
                    std::make_shared<sLRentCell::sMOffering>(sCCI.scc_objCompany, gTile,
                                                             inSCM.sM_groupLand->gL_gTerrain->gTG_TypeGen->get(gTile),
                                                             (uint16_t) cQuality, (uint32_t) cPrice,
                                                             con_TypePaymentFreq::LC_PAY_MONTH);
            inSCM.sM_groupEconomy->addListing<sLRentCell>(sMO);
        }

        //GAIN RENT CELL
        static void
        gExecuteGainRentCellOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                      const uint32_t inRTime, const uint32_t inCDate) {
            sLRentCell::sMFilter sMF(sCCI.scc_addIdInfo);
            std::shared_ptr<sLRentCell::sMOffering> sMOff = inSCM.sM_groupEconomy->getListOfOffering<sLRentCell>(sMF);

            uint32_t uuidContract = inSCM.sM_sContractor->addContractToCompany(sCCI.scc_objCompany,
                                                                               sMOff->sMO_givingCompany, *sMOff,
                                                                               inCDate);
            inSCM.sM_groupEconomy->removeCompleteProcess<sLRentCell>(sMOff);

            std::shared_ptr<sEventManager> sEM = sEventManager::getInstance();
            sEM->callEventCompanyRentCell_PGiving(inRTime, inCDate, sMOff->sMO_givingCompany->c_uuid, uuidContract);
            sEM->callEventCompanyRentCell_PReceiving(inRTime, inCDate, sCCI.scc_objCompany->c_uuid, uuidContract);
        }

        //SELL CELL
        static void
        gExecuteSellCellOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                  const uint32_t inRTime, const uint32_t inCDate) {
            std::pair<int, int> gTile = sCCI.scc_objCompany->getOwnedByIndex(sCCI.scc_addIdInfo);

            uint32_t cQuality = inSCM.sM_groupLand->gL_gTerrain->getQualityGivenPosHome(gTile) -
                                inSCM.sM_groupLand->gL_gAirPollution->getPenalizationAir(gTile);

            uint32_t endRentPrice = getTotalPriceOfHome(gTile, inSCM);
            uint32_t cPrice = inSCM.sM_groupLand->gL_gTerrain->gTG_civilOccupancy->get(gTile) * endRentPrice * 100;
            std::shared_ptr<sLBuyCell::sMOffering> sMO =
                    std::make_shared<sLBuyCell::sMOffering>(sCCI.scc_objCompany, gTile,
                                                            inSCM.sM_groupLand->gL_gTerrain->gTG_TypeGen->get(gTile),
                                                            cQuality, cPrice);
            inSCM.sM_groupEconomy->addListing<sLBuyCell>(sMO);
            inSCM.sM_groupLand->gL_gTerrain->removeBuilding(gTile);
            inSCM.sTComp->removeRefPosOwnCompany(gTile, sCCI.scc_objCompany->c_uuid);
        }

        //PRODUCE OBJ
        static void
        gExecuteProduceObjOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                    const uint32_t inRTime, const uint32_t inCDate) {
            if (!_gProduceProduct(sCCI, inSCM, inRTime, inCDate))
                inSCM.sSCode->updateScoreCode(sCCI.scc_objCompany->c_uuid, -50);
        }

        static bool
        _gProduceProduct(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM, const uint32_t inRTime,
                         const uint32_t inCDate) {
            if (!inSCM.sM_groupEconomy->doesObjectExists(sCCI.scc_addIdInfo))
                return false;
            if (!_hasTypeOwn(sCCI.scc_objCompany, inSCM.sM_groupEconomy->getById(sCCI.scc_addIdInfo)))
                return false;
            if (!_hasResources(*sCCI.scc_objCompany, inSCM.sM_groupEconomy->getById(sCCI.scc_addIdInfo)))
                for (uint32_t mLack: inSCM.sM_groupEconomy->getById(sCCI.scc_addIdInfo).sMEE_iCElem)
                    gTryIntention({sCompanyCompiler::sCCIntentions::OBJ_Buy, mLack, sCCI.scc_objCompany}, inSCM,
                                  inRTime, inCDate);

            for (const auto &gElem: inSCM.sM_groupEconomy->getById(sCCI.scc_addIdInfo).sMEE_iCElem)
                sCCI.scc_objCompany->c_pOwn[gElem] -= 1;
            sCCI.scc_objCompany->c_cAvailableByType[inSCM.sM_groupEconomy->getById(
                    sCCI.scc_addIdInfo).sMEE_iReqTypeBuild] -= 1;
            inSCM.sM_sCompanyTimer->addTimer(sCCI.scc_addIdInfo,
                                             inSCM.sM_groupEconomy->getById(sCCI.scc_addIdInfo).sMEE_iTime + inCDate,
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
                                const uint32_t inRTime, const uint32_t inCDate) {
            inSCM.sM_groupEconomy->computeBoughtElement(sCCI.scc_addIdInfo, sCCI.scc_objCompany, inRTime, inCDate);
        }

        //SELL OBJ
        static void
        gExecuteSellObjOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                 const uint32_t inRTime, const uint32_t inCDate) {
            inSCM.sM_groupEconomy->computeSellInmElement(sCCI.scc_addIdInfo, sCCI.scc_objCompany, inRTime, inCDate);
        }

        //CREATE COMPANY GEN
        static void
        gExecuteCreateCompanyGenOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                          const uint32_t inRTime, const uint32_t inCDate) {
            std::vector<sgTerrain::sgT_TypeGen> gTypeGivenTC = {sgTerrain::sgT_TypeGen::sgT_TG_CivBuilding,
                                                                sgTerrain::sgT_TypeGen::sgT_TG_IndBuilding,
                                                                sgTerrain::sgT_TypeGen::sgT_TG_FieldBuilding};
            sgTerrain::sgT_TypeGen gTypeCompany = gTypeGivenTC[sCCI.scc_addIdInfo % gTypeGivenTC.size()];
            uint32_t uuidNew = inSCM.sTComp->createCompany({}, gTypeCompany);
            gTryIntention({sCompanyCompiler::sCCIntentions::CELL_Buy, gTypeCompany,
                           inSCM.sTComp->getCompanyByUUID(uuidNew)}, inSCM, inRTime, inCDate);
            for (int i = 0; i < 2; i++)
                FulfillIntentions::gTryIntention({sCompanyCompiler::sCCIntentions::GEN_HireEmployee, 0,
                                                  inSCM.sTComp->getCompanyByUUID(uuidNew)}, inSCM, inRTime, inCDate);

            sEventManager::getInstance()->callEventCompanyCreation(inRTime, inCDate, uuidNew);
        }

        static void
        gExecuteHireEmployeeGenOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                         const uint32_t inRTime, const uint32_t inCDate) {
            std::pair<int, int> _homePEmployee = inSCM.sM_sCompEmployee->getNewValidHouse();
            uint32_t _salaryEmployee = 400; // TODO :( Dinamic employee _salary

            if (_salaryEmployee < inSCM.sM_sCompEmployee->getPriceByHouse(_homePEmployee))
                return; //Cannot pay the house

            std::shared_ptr<objCivil> _newCivil = inSCM.sM_sCompEmployee->createCivil(_homePEmployee,
                                                                                      *sCCI.scc_objCompany);
            _assignHouseEmployee(_homePEmployee, inSCM, inCDate);

            inSCM.sM_sContractor->addContractToCompany(sCCI.scc_objCompany, _newCivil, 400, inCDate);

            //Pagar al llogater.
            if (inSCM.sTComp->isCompanyInPosition(_homePEmployee))
                _addContractRentEmployee(_homePEmployee, inSCM, inCDate);

            sEventManager::getInstance()->callEventNewCitizen(inRTime, inCDate, 0); //TODO STORAGE CITIZEN
        }

        static void _assignHouseEmployee(const std::pair<int, int> &inPHouse, sCompanyMain &inSCM, uint32_t inTDate) {
            inSCM.sM_groupEconomy->gE_sRLR->addElement(inTDate);
            inSCM.sM_groupLand->gL_gTerrain->addCivilHomeToPos(inPHouse);
        }

        static void
        _addContractRentEmployee(const std::pair<int, int> &inPHouse, sCompanyMain &inSCM, uint32_t inTDate) {
            uint32_t endRentPrice = getTotalPriceOfHome(inPHouse, inSCM);
            std::shared_ptr<objCompany> _oCRent = inSCM.sTComp->getCompanyByPosition(inPHouse);
            inSCM.sM_sContractor->addContractToCompanyRentHouse(_oCRent, inPHouse,
                                                                endRentPrice, inTDate);
        }

        static void
        gExecuteTerminateEmployeeGenOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                              const uint32_t inRTime, const uint32_t inCDate) {
            std::shared_ptr<objCivil> _removedCivil = inSCM.sM_sCompEmployee->removeEmployeeToCompany(sCCI.scc_objCompany->c_uuid);
            uint32_t _uuidContractTerminate = sCCI.scc_objCompany->c_activeContracts[con_Type::con_Type_Hire].front();
            sCCI.scc_objCompany->c_activeContracts[con_Type::con_Type_Hire].pop_front();
            inSCM.sM_sContractor->removeContractFromCompany(_uuidContractTerminate, inSCM.sTComp);

            if (inSCM.sTComp->isCompanyInPosition(_removedCivil->c_pHome)) {
                std::shared_ptr<objCompany> _oCRent = inSCM.sTComp->getCompanyByPosition(_removedCivil->c_pHome);
                inSCM.sM_sContractor->removeContractFromCompany(
                        _oCRent->c_activeContracts[con_Type::con_Type_RentHome].front(), inSCM.sTComp);
                _oCRent->c_activeContracts[con_Type::con_Type_RentHome].pop_front();
            }

            sEventManager::getInstance()->callEventLeaveCitizen(inRTime, inCDate, 0); //TODO STORAGE CITIZEN
        }
    };
};

#endif //CITYOFWEIRDFISHES_SCOMPANYMAIN_H
#pragma clang diagnostic pop