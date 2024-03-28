//
// Created by Laminar on 15/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASETOWFC_H
#define CITYOFWEIRDFISHES_GBASETOWFC_H

#include <memory>
#include <list>
#include <vector>
#include <random>
#include <map>
#include "../gIGrid.h"
#include "../gBasicGrid.h"
#include "../../../snCommonAtr.h"

template<typename T>
class gBaseToWFC {
private:

    struct lPosActive {
        uint64_t lpaValPiece{};
        uint32_t nValPiece{};
        std::pair<uint32_t, uint32_t> lpaPos;
    };

    std::list<lPosActive> gActivePos;
    std::shared_ptr<gBasicGrid<typename std::list<lPosActive>::iterator>> gIter;
    std::shared_ptr<gIGrid<bool>> gMask;
    std::shared_ptr<gIGrid<bool>> gValid;
    std::shared_ptr<gIGrid<T>> gUsed;
    bool hasMask;


    uint64_t randomSelectOneBit(const uint64_t& value) {
        std::vector<int> bitIndices;
        for (int i = 0; i < 64; ++i) {
            if (value & (1ULL << i)) {
                bitIndices.push_back(i);
            }
        }

        std::mt19937 gen;
        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            gen.seed(snCommonAtr::getFlagAtr("snCA_Seed"));
        else
            gen.seed(static_cast<unsigned int>(time(nullptr)));
        std::uniform_int_distribution<> dis(0, bitIndices.size() - 1);

        int randomIndex = dis(gen);
        return 1ULL << bitIndices[randomIndex];
    }


    typename std::list<lPosActive>::iterator addAndReturn(const lPosActive &lP) {
        auto itNewAdded = std::lower_bound(gActivePos.begin(), gActivePos.end(), lP,
                                           [](const lPosActive &a, const lPosActive &b) {
                                               return a.nValPiece < b.nValPiece;
                                           });
        return gActivePos.insert(itNewAdded, lP);
    }

    void collapse() {
        typename std::list<lPosActive>::iterator itToCol = gActivePos.begin();
        std::vector<std::pair<int, int>> dOffsets =
                {{itToCol->lpaPos.first,     itToCol->lpaPos.second + 1},
                 {itToCol->lpaPos.first - 1, itToCol->lpaPos.second},
                 {itToCol->lpaPos.first + 1, itToCol->lpaPos.second},
                 {itToCol->lpaPos.first,     itToCol->lpaPos.second - 1}};

        itToCol->lpaValPiece = itToCol->nValPiece > 1 ? randomSelectOneBit(itToCol->lpaValPiece) : itToCol->lpaValPiece;
        itToCol->nValPiece = 1;

        gUsed->set(itToCol->lpaPos.first, itToCol->lpaPos.second, log2(itToCol->lpaValPiece) + 1);
        gValid->set(itToCol->lpaPos.first, itToCol->lpaPos.second, false);

        lIntroPiece lPieceDecided = mIntroPiece[log2(itToCol->lpaValPiece) +1];

        for (auto &dOffset: dOffsets) {
            if (gIter->isInside(dOffset.first, dOffset.second) &&
                (!hasMask || gMask->get(dOffset.first, dOffset.second)) &&
                gValid->get(dOffset.first, dOffset.second)) {
                lPosActive lP;
                if (gIter->get(dOffset.first, dOffset.second) == gActivePos.end()) {
                    lP = lPosActive{lPieceDecided.pRel, static_cast<uint32_t>(__builtin_popcountll(lPieceDecided.pRel)), dOffset};
                } else {
                    auto itToRep = gIter->get(dOffset.first, dOffset.second);
                    lP = lPosActive{lPieceDecided.pRel & itToRep->lpaValPiece,
                                    static_cast<uint32_t>(__builtin_popcountll(lPieceDecided.pRel & itToRep->lpaValPiece)), dOffset};
                    gActivePos.erase(itToRep);
                }

                auto itNewAdded = addAndReturn(lP);
                gIter->set(dOffset.first, dOffset.second, itNewAdded);
            }
        }
        gActivePos.erase(itToCol);
    }

public:
    struct lIntroPiece {
        uint8_t pId;
        uint64_t pRel;
    };

    gBaseToWFC(std::shared_ptr<gIGrid<T>> pUsed,
               std::list<lIntroPiece> lRules,
               std::pair<uint32_t, uint32_t> sPos, uint8_t sId, const std::shared_ptr<gIGrid<bool>> &pMask = nullptr)
            : gUsed(pUsed), gMask(pMask), hasMask(pMask != nullptr) {
        for (const lIntroPiece &ip: lRules)
            mIntroPiece[ip.pId] = ip;

        gIter = std::make_shared<gBasicGrid<typename std::list<lPosActive>::iterator>>(
                gBasicGrid<typename std::list<lPosActive>::iterator>(pUsed->rangeUse().first.second -
                                                                     pUsed->rangeUse().first.first + 1,
                                                                     pUsed->rangeUse().second.second -
                                                                     pUsed->rangeUse().second.first + 1,
                                                                     gActivePos.end()));
        gValid = std::make_shared<gBasicGrid<bool>>(
                gBasicGrid<bool>(pUsed->rangeUse().first.second -
                                 pUsed->rangeUse().first.first + 1,
                                 pUsed->rangeUse().second.second -
                                 pUsed->rangeUse().second.first + 1,
                                 true));
        lPosActive lP{sId, 1, sPos};

        auto it = addAndReturn(lP);
        gIter->set(sPos.first, sPos.second, it);

        while (!gActivePos.empty()) {
            collapse();
        }
    }

private:
    std::map<uint8_t, lIntroPiece> mIntroPiece;
};

#endif //CITYOFWEIRDFISHES_GBASETOWFC_H
