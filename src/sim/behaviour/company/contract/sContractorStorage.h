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
    void addCon(std::shared_ptr<con_rentCell> &con) {
        gTypeAndIndex.emplace_back(0, gVecConRent.size());
        gVecConRent.push_back(con);
        con->ct_uuid = gTypeAndIndex.size();
    }

    void addCon(std::shared_ptr<con_buyCell> &con) {
        gTypeAndIndex.emplace_back(1, gVecConBuy.size());
        gVecConBuy.push_back(con);
        con->ct_uuid = gTypeAndIndex.size();
    }

    void addCon(std::shared_ptr<con_loanInteraction> &con) {
        gTypeAndIndex.emplace_back(2, gVecConLoan.size());
        gVecConLoan.push_back(con);
        con->ct_uuid = gTypeAndIndex.size();
    }

    void addCon(std::shared_ptr<con_stockInteraction> &con) {
        gTypeAndIndex.emplace_back(3, gVecConStock.size());
        gVecConStock.push_back(con);
        con->ct_uuid = gTypeAndIndex.size();
    }

    std::shared_ptr<con_b> getConByUuid(uint64_t uuidCon) {
        switch (gTypeAndIndex[uuidCon].first) {
            case 0:
                return gVecConRent[gTypeAndIndex[uuidCon].second];
            case 1:
                return gVecConBuy[gTypeAndIndex[uuidCon].second];
            case 2:
                return gVecConLoan[gTypeAndIndex[uuidCon].second];
            case 3:
                return gVecConStock[gTypeAndIndex[uuidCon].second];
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

private:
    std::vector<std::pair<uint8_t, uint32_t>> gTypeAndIndex;
    std::vector<std::shared_ptr<con_rentCell>> gVecConRent;
    std::vector<std::shared_ptr<con_buyCell>> gVecConBuy;
    std::vector<std::shared_ptr<con_loanInteraction>> gVecConLoan;
    std::vector<std::shared_ptr<con_stockInteraction>> gVecConStock;
};

#endif //CITYOFWEIRDFISHES_SCONTRACTORSTORAGE_H
