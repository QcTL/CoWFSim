//
// Created by ganymede on 3/19/24.
//

#ifndef CITYOFWEIRDFISHES_SCONTRACTORSTORAGE_H
#define CITYOFWEIRDFISHES_SCONTRACTORSTORAGE_H

#include <cstdint>
#include <vector>
#include "../../market/listings/implementations/sLRentCell.h"

class sContractorStorage {
public:
    [[nodiscard]] uint32_t addCon(std::shared_ptr<con_rentCell> &con) {
        gTypeAndIndex.emplace_back(con_Type::con_Type_Rent, gVecConRent.size());
        gVecConRent.push_back(con);
        con->ct_uuid = gTypeAndIndex.size();
        return gVecConRent.size() - 1;
    }

    [[nodiscard]] uint32_t addCon(std::shared_ptr<con_buyCell> &con) {
        gTypeAndIndex.emplace_back(con_Type::con_Type_Buy, gVecConBuy.size());
        gVecConBuy.push_back(con);
        con->ct_uuid = gTypeAndIndex.size();
        return gVecConBuy.size() - 1;
    }

    [[nodiscard]] uint32_t addCon(std::shared_ptr<con_loanInteraction> &con) {
        gTypeAndIndex.emplace_back(con_Type::con_Type_Loan, gVecConLoan.size());
        gVecConLoan.push_back(con);
        con->ct_uuid = gTypeAndIndex.size();
        return gVecConLoan.size() - 1;
    }

    [[nodiscard]] uint32_t addCon(std::shared_ptr<con_stockInteraction> &con) {
        gTypeAndIndex.emplace_back(con_Type::con_Type_Stock, gVecConStock.size());
        gVecConStock.push_back(con);
        con->ct_uuid = gTypeAndIndex.size();
        return gVecConStock.size() - 1;
    }

    [[nodiscard]] uint32_t addCon(std::shared_ptr<con_hireInteraction> &con) {
        gTypeAndIndex.emplace_back(con_Type::con_Type_Hire, gVecConHire.size());
        gVecConHire.push_back(con);
        con->ct_uuid = gTypeAndIndex.size();
        return gVecConHire.size() - 1;
    }

    [[nodiscard]] uint32_t addCon(std::shared_ptr<con_rentHouseInteraction> &con) {
        gTypeAndIndex.emplace_back(con_Type::con_Type_RentHome, gVecConRentHome.size());
        gVecConRentHome.push_back(con);
        con->ct_uuid = gTypeAndIndex.size();
        return gVecConRentHome.size() - 1;
    }

    [[nodiscard]] std::shared_ptr<con_b> getConByUuid(uint64_t uuidCon) {
        switch (gTypeAndIndex[uuidCon].first) {
            case con_Type::con_Type_Rent:
                return gVecConRent[gTypeAndIndex[uuidCon].second];
            case con_Type::con_Type_Buy:
                return gVecConBuy[gTypeAndIndex[uuidCon].second];
            case con_Type::con_Type_Loan:
                return gVecConLoan[gTypeAndIndex[uuidCon].second];
            case con_Type::con_Type_Stock:
                return gVecConStock[gTypeAndIndex[uuidCon].second];
            case con_Type::con_Type_Hire:
                return gVecConHire[gTypeAndIndex[uuidCon].second];
            case con_Type::con_Type_RentHome:
                return gVecConRentHome[gTypeAndIndex[uuidCon].second];
            default:
                return nullptr;
        }
    }

    uint8_t getTypeByUuid(uint64_t uuidCon) {
        return gTypeAndIndex[uuidCon].first;
    }

    con_rentCell getRentCellByUuid(
            uint64_t uuidCon) { return reinterpret_cast<con_rentCell &&>(gVecConRent[gTypeAndIndex[uuidCon].second]); }

    con_buyCell getBuyCellByUuid(
            uint64_t uuidCon) { return reinterpret_cast<con_buyCell &&>(gVecConBuy[gTypeAndIndex[uuidCon].second]); }

    con_loanInteraction getLoanCellByUuid(
            uint64_t uuidCon) { return reinterpret_cast<con_loanInteraction &&>(gVecConLoan[gTypeAndIndex[uuidCon].second]); }

    con_stockInteraction getStockCellByUuid(
            uint64_t uuidCon) { return reinterpret_cast<con_stockInteraction &&>(gVecConStock[gTypeAndIndex[uuidCon].second]); }

    con_hireInteraction getHireByUuid(
            uint64_t uuidCon) { return reinterpret_cast<con_hireInteraction &&>(gVecConHire[gTypeAndIndex[uuidCon].second]); }

    con_rentHouseInteraction getRentHomeByUuid(
            uint64_t uuidCon) { return reinterpret_cast<con_rentHouseInteraction &&>(gVecConRentHome[gTypeAndIndex[uuidCon].second]); }

private:
    std::vector<std::pair<uint8_t, uint32_t>> gTypeAndIndex;
    std::vector<std::shared_ptr<con_rentCell>> gVecConRent;
    std::vector<std::shared_ptr<con_buyCell>> gVecConBuy;
    std::vector<std::shared_ptr<con_loanInteraction>> gVecConLoan;
    std::vector<std::shared_ptr<con_stockInteraction>> gVecConStock;
    std::vector<std::shared_ptr<con_hireInteraction>> gVecConHire;
    std::vector<std::shared_ptr<con_rentHouseInteraction>> gVecConRentHome;
};

#endif //CITYOFWEIRDFISHES_SCONTRACTORSTORAGE_H
