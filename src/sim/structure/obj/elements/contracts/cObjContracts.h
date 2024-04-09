//
// Created by Laminar on 17/03/2024.
//

#ifndef CITYOFWEIRDFISHES_COBJCONTRACTS_H
#define CITYOFWEIRDFISHES_COBJCONTRACTS_H

#include <cstdint>
#include <vector>
#include "../objCompany.h"

enum con_TypePaymentFreq {
    LC_PAY_MONTH, LC_PAY_YEAR, LC_PAY_FORTNIGHT
};

enum con_Type {
    con_Type_Rent = 0,
    con_Type_Buy = 1,
    con_Type_Loan = 2,
    con_Type_Stock = 3,
    con_Type_Hire = 4,
    con_Type_RentHome = 5,
};


class con_b {
public:
    uint64_t ct_uuid = 0;
    int ct_uuidCompanyGiving;
    uint32_t ct_strDate;

    bool ct_hasEndDate;
    bool ct_hasReceiving;

    uint32_t ct_endDate = 0;
    int ct_uuidCompanyReceiving = 0;

    con_b(int ctUuidCompanyGiving, uint32_t ctStrDate)
            : ct_strDate(ctStrDate),
              ct_uuidCompanyGiving(ctUuidCompanyGiving),
              ct_hasEndDate(false), ct_hasReceiving(false) {}

    con_b(int ctUuidCompanyGiving, int ctUuidCompanyReceiving, uint32_t ctStrDate)
            : ct_strDate(ctStrDate),
              ct_uuidCompanyGiving(ctUuidCompanyGiving),
              ct_hasEndDate(false),
              ct_hasReceiving(true), ct_uuidCompanyReceiving(ctUuidCompanyReceiving) {}

    con_b(int ctUuidCompanyGiving, int ctUuidCompanyReceiving, uint32_t ctStrDate, uint32_t ctEndDate)
            : ct_strDate(ctStrDate),
              ct_uuidCompanyGiving(ctUuidCompanyGiving),
              ct_hasEndDate(true), ct_endDate(ctEndDate),
              ct_hasReceiving(true), ct_uuidCompanyReceiving(ctUuidCompanyReceiving) {}

    virtual void
    addAsReceiving(const std::shared_ptr<objCompany> &objCom) = 0;

    virtual void
    addAsGiving(const std::shared_ptr<objCompany> &objCom) = 0;

    virtual void
    removeAsReceiving(const std::shared_ptr<objCompany> &objCom) = 0;

    virtual void
    removeAsGiving(const std::shared_ptr<objCompany> &objCom) = 0;

    static void
    addPropertyContract(const std::shared_ptr<objCompany> &objCom, con_TypePaymentFreq c_TPF, float ct_recurrentCost,
                        bool isGaining) {
        switch (c_TPF) {
            case LC_PAY_MONTH:
                objCom->c_objMonth.set(
                        isGaining ? objCom->c_objMonth + ct_recurrentCost : objCom->c_objMonth - ct_recurrentCost);
                break;
            case LC_PAY_YEAR:
                objCom->c_objYear.set(
                        isGaining ? objCom->c_objYear + ct_recurrentCost : objCom->c_objYear - ct_recurrentCost);
                break;
            case LC_PAY_FORTNIGHT:
                objCom->c_objWeek.set(
                        isGaining ? objCom->c_objWeek + ct_recurrentCost : objCom->c_objWeek - ct_recurrentCost);
                break;
        }
    }
};

class con_rentCell : public con_b {
public:
    con_TypePaymentFreq ct_typePayment;
    uint32_t ct_recurrentCost;
    uint32_t ct_totalCost;
    std::list<std::pair<int, int>> ct_lCells;
    uint8_t ct_typeCell;

    con_rentCell(uint32_t ctUuidCompanyReceiving, uint32_t ctUuidCompanyGiving, uint32_t ctStrDate, uint32_t ctEndDate,
                 con_TypePaymentFreq ctTypePayment, uint32_t ctRecurrentCost, uint32_t ctTotalCost,
                 const std::list<std::pair<int, int>> &ctLCells, uint8_t typeCell) : con_b(ctUuidCompanyGiving,
                                                                                           ctUuidCompanyReceiving,
                                                                                           ctStrDate, ctEndDate),
                                                                                     ct_typePayment(ctTypePayment),
                                                                                     ct_recurrentCost(ctRecurrentCost),
                                                                                     ct_totalCost(ctTotalCost),
                                                                                     ct_lCells(ctLCells),
                                                                                     ct_typeCell(typeCell) {}

    //TODO AFEGIR CANVIS A C_RENTEDlOCATIONS
    void
    addAsReceiving(const std::shared_ptr<objCompany> &objCom) override {
        objCom->addAvailableLocation(ct_lCells, ct_typeCell);
        addPropertyContract(objCom, ct_typePayment, ct_recurrentCost, false);
    };

    void
    addAsGiving(const std::shared_ptr<objCompany> &objCom) override {
        if (objCom == nullptr)
            return;
        objCom->removeAvailableLocation(ct_lCells, ct_typeCell);
        addPropertyContract(objCom, ct_typePayment, ct_recurrentCost, true);
    };

    void
    removeAsReceiving(const std::shared_ptr<objCompany> &objCom) override {
        addAsGiving(objCom);
    };

    void
    removeAsGiving(const std::shared_ptr<objCompany> &objCom) override {
        if (objCom == nullptr)
            return;
        addAsReceiving(objCom);
    };
};


class con_buyCell : public con_b {
public:
    con_TypePaymentFreq ct_typePayment;
    uint32_t ct_recurrentCost;
    uint32_t ct_totalCost;
    std::list<std::pair<int, int>> ct_lCells;
    uint8_t ct_typeCell;

    con_buyCell(uint32_t ctUuidCompanyReceiving, uint32_t ctUuidCompanyGiving, uint32_t ctStrDate,
                con_TypePaymentFreq ctTypePayment, uint32_t ctRecurrentCost, uint32_t ctTotalCost,
                const std::list<std::pair<int, int>> &ctLCells, uint8_t typeCell)
            : con_b(ctUuidCompanyGiving, ctUuidCompanyReceiving, ctStrDate),
              ct_typePayment(ctTypePayment),
              ct_recurrentCost(ctRecurrentCost),
              ct_totalCost(ctTotalCost), ct_lCells(ctLCells), ct_typeCell(typeCell) {}

    void
    addAsReceiving(const std::shared_ptr<objCompany> &objCom) override {
        objCom->addAvailableLocation(ct_lCells, ct_typeCell);
        addPropertyContract(objCom, ct_typePayment, ct_recurrentCost, false);
    };

    void
    addAsGiving(const std::shared_ptr<objCompany> &objCom) override {
        if (objCom == nullptr)
            return;
        objCom->removeAvailableLocation(ct_lCells, ct_typeCell);
        addPropertyContract(objCom, ct_typePayment, ct_recurrentCost, true);
    };

    void
    removeAsReceiving(const std::shared_ptr<objCompany> &objCom) override {
        addAsGiving(objCom);
    };

    void
    removeAsGiving(const std::shared_ptr<objCompany> &objCom) override {
        if (objCom == nullptr)
            return;
        addAsReceiving(objCom);
    };
};

class con_loanInteraction : public con_b {
public:
    con_TypePaymentFreq ct_typePayment;

    uint32_t ct_askedStart;
    uint32_t ct_recurrentCostReturn;
    uint32_t ct_totalCostReturn;

    con_loanInteraction(uint32_t ctUuidCompanyReceiving, uint32_t ctUuidCompanyGiving, uint32_t ctStrDate,
                        con_TypePaymentFreq ctTypePayment, uint32_t askedStart, uint32_t ctRecurrentCost,
                        uint32_t ctTotalCost)
            : con_b(ctUuidCompanyGiving, ctUuidCompanyReceiving, ctStrDate),
              ct_typePayment(ctTypePayment),
              ct_recurrentCostReturn(ctRecurrentCost),
              ct_totalCostReturn(ctTotalCost),
              ct_askedStart(askedStart) {}

    void
    addAsReceiving(const std::shared_ptr<objCompany> &objCom) override {
        objCom->c_cActiveFunds += ct_askedStart;
        addPropertyContract(objCom, ct_typePayment, ct_recurrentCostReturn, false);
    };

    void
    addAsGiving(const std::shared_ptr<objCompany> &objCom) override {
        objCom->c_cActiveFunds -= ct_askedStart;
        addPropertyContract(objCom, ct_typePayment, ct_recurrentCostReturn, true);
    };

    void
    removeAsReceiving(const std::shared_ptr<objCompany> &objCom) override {
        addPropertyContract(objCom, ct_typePayment, ct_recurrentCostReturn, true);
    };

    void
    removeAsGiving(const std::shared_ptr<objCompany> &objCom) override {
        addPropertyContract(objCom, ct_typePayment, ct_recurrentCostReturn, false);
    };
};

class con_stockInteraction : public con_b {
public:
    uint32_t ct_boughtStock;
    double ct_relativeTotalStock;

    con_stockInteraction(uint32_t ctUuidCompanyReceiving, uint32_t ctUuidCompanyGiving, uint32_t ctStrDate,
                         uint32_t boughtStock, double relativeTotalStock)
            : con_b(ctUuidCompanyGiving, ctUuidCompanyReceiving, ctStrDate),
              ct_boughtStock(boughtStock),
              ct_relativeTotalStock(relativeTotalStock) {}

    //TODO!!! STOCKS
    void
    addAsReceiving(const std::shared_ptr<objCompany> &objCom) override {};

    void
    addAsGiving(const std::shared_ptr<objCompany> &objCom) override {};

    void
    removeAsReceiving(const std::shared_ptr<objCompany> &objCom) override {};

    void
    removeAsGiving(const std::shared_ptr<objCompany> &objCom) override {};
};

class con_hireInteraction : public con_b {
public:
    uint32_t ct_UuidCivil;
    uint32_t ct_salary;

    con_hireInteraction(uint32_t ctUuidCompanyGiving, uint32_t ctUuidCivil, uint32_t ctSalary,
                        uint32_t ctStrDate)
            : con_b(ctUuidCompanyGiving, ctStrDate), ct_UuidCivil(ctUuidCivil), ct_salary(ctSalary) {}

    void
    addAsReceiving(const std::shared_ptr<objCompany> &objCom) override {};

    void
    addAsGiving(const std::shared_ptr<objCompany> &objCom) override {
        objCom->c_objMonth -= (float) ct_salary;
        objCom->c_nEmployee += 1;
    };

    void
    removeAsReceiving(const std::shared_ptr<objCompany> &objCom) override {};

    void
    removeAsGiving(const std::shared_ptr<objCompany> &objCom) override {
        objCom->c_objMonth += (float) ct_salary;
        objCom->c_nEmployee -= 1;
    };
};

class con_rentHouseInteraction : public con_b {
public:
    std::pair<int, int> ct_home;
    uint32_t ct_cost;

    con_rentHouseInteraction(uint32_t ctUuidCompanyGiving, const std::pair<int, int> &ctHome, uint32_t ctCost,
                             uint32_t ctStrDate)
            : con_b(ctUuidCompanyGiving, ctStrDate), ct_home(ctHome), ct_cost(ctCost) {}

    void
    addAsReceiving(const std::shared_ptr<objCompany> &objCom) override {
    };

    void
    addAsGiving(const std::shared_ptr<objCompany> &objCom) override {
        objCom->c_objMonth += (float) ct_cost;
    };

    void
    removeAsReceiving(const std::shared_ptr<objCompany> &objCom) override {};

    void
    removeAsGiving(const std::shared_ptr<objCompany> &objCom) override {
        objCom->c_objMonth -= (float) ct_cost;
    };
};

#endif //CITYOFWEIRDFISHES_COBJCONTRACTS_H
