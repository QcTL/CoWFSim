#pragma clang diagnostic push
#pragma ide diagnostic ignored "ArgumentSelectionDefects"
//
// Created by Laminar on 05/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMPANYMAIN_H
#define CITYOFWEIRDFISHES_SCOMPANYMAIN_H


#include "sCompanyStorage.h"
#include "../../structure/grids/gIGrid.h"
#include "../../structure/grids/gBasicGrid.h"
#include "employee/sCivilMain.h"
#include "../../groups/groupEconomy/groupEconomy.h"
#include "contract/sContractorMain.h"
#include "../globalAttr/stGlobalTrackerAttr.h"

/**
 * @class sCompanyMain
 * @brief Class that contains all the interaction with the companies and applies their actions on the simulation
 */
class sCompanyMain {
public:

    enum sCM_strStyleCompany {
        SCM_strOffice = 0, SCM_strFactory = 1, SCM_strField = 2
    };

    explicit sCompanyMain(uint32_t inGridSize, const std::shared_ptr<sCivilMain> &inPCivilMain,
                          const std::shared_ptr<groupLand> &inGLand, const std::shared_ptr<groupEconomy> &inGEconomy)
            : sM_groupLand(inGLand), sM_sCompEmployee(inPCivilMain), sM_groupEconomy(inGEconomy) {
        sTComp = std::make_shared<sCompanyStorage>(inGridSize, 5000);
        sM_sContractor = std::make_shared<sContractorMain>();

        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            sCM_genRPos.seed(snCommonAtr::getFlagAtr("snCA_Seed"));
        else
            sCM_genRPos.seed(static_cast<unsigned int>(time(nullptr)));
    }

    /**
     * @fn void tickReduced
     * @brief Updates the elements in the class that need changing every 5 tick of the simulation
     * @param inRTime uint representing the reduced time of the day
     * @param inTDate uint representing the reduced date of the simulation
     */
    void tickReduced(const uint32_t inRTime, const uint32_t inTDate) {
        if (sM_groupEconomy->gE_sEvaluator->someCompletedProducts(inTDate))
            for (std::pair<uint32_t, uint32_t> &t: sM_groupEconomy->gE_sEvaluator->getCompletedProducts(inTDate))
                FulfillIntentions::gCompletedProduct(sTComp->getCompanyByUUID(t.second),
                                                     t.first, sM_groupEconomy, inTDate);

        if (inRTime == 0) {
            std::uniform_int_distribution<int> distribution(0,
                                                            std::max(2, std::min(100, (int) (100000 /
                                                                                             sM_sCompEmployee->getNCivil()))));
            int _resChoose = distribution(sCM_genRPos);
            if (_resChoose <= 2) {
                addNewCompany(static_cast<sCM_strStyleCompany>(_resChoose), inRTime, inTDate);
                std::cout << "S'HA CREAT UNA EMPRESA" << std::endl;
            }
        }

        if (inRTime % (12 * 3) == 0) { //ONCE EVERY 3 HOURS
            for (const sCompanyCompiler::sCCIntentions &sCCI: sTComp->getTotalIntentions(inRTime)) {
                FulfillIntentions::gTryIntention(sCCI, *this, inRTime, inTDate);
                sTComp->updateScoreCode(sCCI.scc_objCompany->c_uuid, -1);
            }

            //Requisits civilians
            std::vector<uint32_t> _sReqProduct =
                    sM_groupEconomy->gE_sEvaluator->getVecConsumeProductsByPopulation(sM_sCompEmployee->getNCivil());
            for (int i = 0; i < _sReqProduct.size(); i++)
                sM_groupEconomy->computeBoughtElementCiv(i, _sReqProduct[i], inRTime, inTDate);

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

        if (inRTime == 0 && inTDate != 0) {
            if ((inTDate & 0x1E0) == 0 && (inTDate & 0x7) == 0 && (inTDate & 0x18) == 0) {
                sTComp->applyObligations(objCompany::objC_TimeObligations::objC_TO_YEAR, inTDate);
                sTComp->applyObligations(objCompany::objC_TimeObligations::objC_TO_MONTH, inTDate);
                sTComp->applyObligations(objCompany::objC_TimeObligations::objC_TO_WEEK, inTDate);
            } else if ((inTDate & 0x18) == 0 && (inTDate & 0x7) == 0) {
                sTComp->applyObligations(objCompany::objC_TimeObligations::objC_TO_MONTH, inTDate);
                sTComp->applyObligations(objCompany::objC_TimeObligations::objC_TO_WEEK, inTDate);
            } else if ((inTDate & 0x7) == 0) {
                sTComp->applyObligations(objCompany::objC_TimeObligations::objC_TO_WEEK, inTDate);
            }
        }

        //Start bankruptcy
        std::vector<std::shared_ptr<objCompany>> vCompBankruptcy = sTComp->getVecCompBankruptcy();
        for (const std::shared_ptr<objCompany> &oCompany: vCompBankruptcy) {
            for (int i = 0; i < oCompany->getNumberActiveCells(); i++)
                FulfillIntentions::gTryIntention(
                        {sCompanyCompiler::sCCIntentions::CELL_Sell, (uint32_t) oCompany->getTypeByIndex(i),
                         oCompany}, *this, 0, inTDate);
            for (int i = 0; i < oCompany->c_nEmployee; i++)
                FulfillIntentions::gTryIntention({sCompanyCompiler::sCCIntentions::GEN_TerminateEmployee, 0,
                                                  oCompany}, *this, inRTime, inTDate);
            sTComp->removeCompany(oCompany);
        }
    }

    /**
     * @fn void completedStartCompanies
     * @brief Create and instance in the simulation all the companies that have to be present since the beginning
     * @param gPosCompanies A vector of elements that describe starting state for each company
     */
    void completedStartCompanies(const std::list<sgTerrain::sgT_CellSlot> &gPosCompanies) {
        for (const auto &posNewComp: gPosCompanies) {
            uint32_t uuidNew = sTComp->createCompany(posNewComp.sgT_gPos, posNewComp.sgT_soilType, 0, 0);
            auto typeSoil = sM_groupLand->gL_gTerrain->gTG_TypeSoil->get(posNewComp.sgT_gPos.front());
            if (typeSoil != sgTerrain::sgT_TypeSoil::sgT_TS_T2Mixed &&
                typeSoil != sgTerrain::sgT_TypeSoil::sgT_TS_T3Mixed) {
                for (int i = 0; i < 2; i++)
                    FulfillIntentions::gTryIntention({sCompanyCompiler::sCCIntentions::GEN_HireEmployee, 0,
                                                      sTComp->getCompanyByUUID(uuidNew)}, *this, 0, 0); //TODO DATE;
            } else
                sTComp->getCompanyByUUID(uuidNew)->c_attrCanEmployee = false;

            sTComp->getCompanyByUUID(uuidNew)->c_objMonth -= 100;
            //TODO, haurien de fer-se un contracte de ownership de alguna manera aixi que no es faria aquí.
            for (const auto p: posNewComp.sgT_gPos)
                FulfillIntentions::doRentContractsExistingResidents(p, *this, 0);
        }
    }

    /**
     * @fn void addNewCompany
     * @brief creates a new company given the style, and the date and time of the actual simulation
     * @param cCompanyCreation An enumerator descriving the type of company that will be created
     * @param inRTime The reduced time of the actual simulation < 288
     * @param inTDate The reduced date where this creation happens, it has to be a valid reduced date
     */
    void addNewCompany(sCM_strStyleCompany cCompanyCreation, const uint32_t inRTime, const uint32_t inTDate) {
        FulfillIntentions::gTryIntention(
                {sCompanyCompiler::sCCIntentions::GEN_CreateCompany, cCompanyCreation, nullptr}, *this, +
                        inRTime, inTDate);
    }

    //STORAGE
    std::shared_ptr<sCompanyStorage> sTComp;
private:
    std::mt19937 sCM_genRPos;
    std::shared_ptr<groupLand> sM_groupLand;
    std::shared_ptr<groupEconomy> sM_groupEconomy;

    std::shared_ptr<sCivilMain> sM_sCompEmployee;
    std::shared_ptr<sContractorMain> sM_sContractor;

    /**
     * @class FulfillIntentions
     * @brief This class is used to explicitly describe how the different intentions have to be executed in the simulation
     */
    class FulfillIntentions {
    public:

        /**
         * @fn bool gTryIntention
         * @param sCCI The type of intention that wil be executed
         * @param inSCM The companyMain containing all the attributes to interact with the simulation
         * @param inRTime The reduced time of the actual simulation < 288
         * @param inTDate The reduced date where this creation happens, it has to be a valid reduced date
         * @return A bool representing if the intention has been correctly executed
         */
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

        /**
         * @fn void gCompletedProduct
         * @brief a function to obtain the product that you queued up days before
         * @param oC A shared pointer to the company you want to obtain the completed products
         * @param gItemGen The item that has produced, it has to be a valid index of the item list, smaller that the last item
         * @param inGEconomy A shared pointer to economy where the changes will be applied
         * @param inTDate The reduced date where this creation happens, it has to be a valid reduced date
         */
        static void gCompletedProduct(const std::shared_ptr<objCompany> &oC, uint32_t gItemGen,
                                      std::shared_ptr<groupEconomy> &inGEconomy, uint32_t inTDate) {
            if (oC != nullptr) {
                inGEconomy->computeCreatedElement(gItemGen, oC);
                oC->c_cAvailableByType[inGEconomy->getById(gItemGen).sMEE_iReqTypeBuild] += 1;

                oC->removeProcessing(inTDate, gItemGen);
            }
        }

        /**
         * @fn void doRentContractsExistingResidents
         * @brief For existing house with residents without a company, that in some point it gains it, this function
         * applies the correct current rents to the finances of the company
         * @param inPHouse A pair of positive coordinates marking the position of a valid house
         * @param inSCM The companyMain containing all the attributes to interact with the simulation
         * @param inTDate The reduced date where this creation happens, it has to be a valid reduced date
         */
        static void
        doRentContractsExistingResidents(const std::pair<int, int> &inPHouse, sCompanyMain &inSCM, uint32_t inTDate) {
            uint8_t nResidents = inSCM.sM_groupLand->gL_gTerrain->gTG_civilOccupancy->get(inPHouse);
            if (nResidents <= 0)
                return;
            for (int i = 0; i < nResidents; i++) {
                _addContractRentHomeEmployee(inPHouse, inSCM, inTDate);
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
                if (sMOff->sMO_givingCompany != nullptr)
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
            if (!sCCI.scc_objCompany->hasOwnedType(sCCI.scc_addIdInfo))
                return;

            std::pair<int, int> gTile = sCCI.scc_objCompany->getOwnedByType(sCCI.scc_addIdInfo);
            sCCI.scc_objCompany->addRentedLocationByPos(gTile, sCCI.scc_addIdInfo);
            uint32_t cQuality = inSCM.sM_groupLand->gL_gTerrain->getQualityGivenPosHome(gTile) -
                                inSCM.sM_groupLand->gL_gAirPollution->getPenalizationAir(gTile);

            uint32_t endRentPrice = getTotalPriceOfHome(gTile, inSCM);
            auto cPrice = (uint32_t) (inSCM.sM_groupLand->gL_gTerrain->gTG_civilOccupancy->get(gTile) * endRentPrice *
                                      1.5);

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
            std::pair<int, int> gTile = sCCI.scc_objCompany->getOwnedByType(sCCI.scc_addIdInfo);

            uint32_t cQuality = inSCM.sM_groupLand->gL_gTerrain->getQualityGivenPosHome(gTile) -
                                inSCM.sM_groupLand->gL_gAirPollution->getPenalizationAir(gTile);

            uint32_t endRentPrice = 0;
            if (sCCI.scc_addIdInfo == sgTerrain::sgT_TypeSoil::sgT_TS_T1Mixed ||
                sCCI.scc_addIdInfo == sgTerrain::sgT_TypeSoil::sgT_TS_T2Mixed ||
                sCCI.scc_addIdInfo == sgTerrain::sgT_TypeSoil::sgT_TS_T3Mixed)
                endRentPrice = getTotalPriceOfHome(gTile, inSCM);

            uint32_t cPrice =
                    inSCM.sM_groupLand->gL_gTerrain->gTG_civilOccupancy->get(gTile) * endRentPrice * 100 + 5000;
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
            if (!inSCM.sM_groupEconomy->gE_sEvaluator->canCompanyProduceObject(sCCI.scc_objCompany,
                                                                               sCCI.scc_addIdInfo)) {
                inSCM.sTComp->updateScoreCode(sCCI.scc_objCompany->c_uuid, -50);
                return;
            }

            for (uint32_t mLack: inSCM.sM_groupEconomy->gE_sEvaluator->getVecMissingProducts(sCCI.scc_objCompany,
                                                                                             sCCI.scc_addIdInfo)) {
                gTryIntention({sCompanyCompiler::sCCIntentions::OBJ_Buy, mLack, sCCI.scc_objCompany}, inSCM,
                              inRTime, inCDate);
            }

            inSCM.sM_groupEconomy->gE_sEvaluator->consumeMaterialsProduceObject(sCCI.scc_objCompany, sCCI.scc_addIdInfo,
                                                                                inCDate);
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
            std::vector<sgTerrain::sgT_TypeSoil> gTypeGivenTC = {sgTerrain::sgT_TypeSoil::sgT_TS_T1Mixed,
                                                                 sgTerrain::sgT_TypeSoil::sgT_TS_T1Industrial,
                                                                 sgTerrain::sgT_TypeSoil::sgT_TS_T1Farm};
            sgTerrain::sgT_TypeSoil gTypeCompany = gTypeGivenTC[sCCI.scc_addIdInfo % gTypeGivenTC.size()];
            uint32_t uuidNew = inSCM.sTComp->createCompany({}, gTypeCompany, inRTime, inCDate);
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

            if (_salaryEmployee < getTotalPriceOfHome(_homePEmployee, inSCM))
                return; //Cannot pay the house

            std::shared_ptr<objCivil> _newCivil = inSCM.sM_sCompEmployee->createCivil(_homePEmployee,
                                                                                      *sCCI.scc_objCompany, inCDate);

            inSCM.sM_sContractor->addContractToCompany(sCCI.scc_objCompany, _newCivil, 400, inCDate);

            //Pagar al llogater.
            if (inSCM.sTComp->isCompanyInPosition(_homePEmployee))
                _addContractRentHomeEmployee(_homePEmployee, inSCM, inCDate);

            sEventManager::getInstance()->callEventNewCitizen(inRTime, inCDate, _newCivil->c_uuid);
        }

        static void
        _addContractRentHomeEmployee(const std::pair<int, int> &inPHouse, sCompanyMain &inSCM, uint32_t inTDate) {
            uint32_t endRentPrice = getTotalPriceOfHome(inPHouse, inSCM);
            std::shared_ptr<objCompany> _oCRent = inSCM.sTComp->getCompanyByPosition(inPHouse);
            inSCM.sM_sContractor->addContractToCompanyRentHouse(_oCRent, inPHouse,
                                                                endRentPrice, inTDate);
        }

        static void
        gExecuteTerminateEmployeeGenOperation(const sCompanyCompiler::sCCIntentions &sCCI, sCompanyMain &inSCM,
                                              const uint32_t inRTime, const uint32_t inCDate) {
            std::shared_ptr<objCivil> _removedCivil = inSCM.sM_sCompEmployee->getCivGivenCompany(
                    sCCI.scc_objCompany->c_uuid);
            uint32_t _uuidContractTerminate = sCCI.scc_objCompany->c_activeContracts[con_Type::con_Type_Hire].front();
            sCCI.scc_objCompany->c_activeContracts[con_Type::con_Type_Hire].pop_front();
            inSCM.sM_sContractor->removeContractFromCompany(_uuidContractTerminate, inSCM.sTComp);

            if (inSCM.sTComp->isCompanyInPosition(_removedCivil->c_pHome)) {
                std::shared_ptr<objCompany> _oCRent = inSCM.sTComp->getCompanyByPosition(_removedCivil->c_pHome);
                inSCM.sM_sContractor->removeContractFromCompany(
                        _oCRent->c_activeContracts[con_Type::con_Type_RentHome].front(), inSCM.sTComp);
                _oCRent->c_activeContracts[con_Type::con_Type_RentHome].pop_front();
            }

            sEventManager::getInstance()->callEventLeaveCitizen(inRTime, inCDate,
                                                                _removedCivil->c_uuid);

            inSCM.sM_sCompEmployee->removeRouteGivenCivil(_removedCivil, sCCI.scc_objCompany->c_uuid);
            inSCM.sM_sCompEmployee->deleteCivil(_removedCivil, sCCI.scc_objCompany->c_uuid);
        }
    };
};

#endif //CITYOFWEIRDFISHES_SCOMPANYMAIN_H
#pragma clang diagnostic pop