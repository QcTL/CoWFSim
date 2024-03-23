//
// Created by ganymede on 3/22/24.
//

#ifndef CITYOFWEIRDFISHES_SROLLINGLISTSEVALUATOR_H
#define CITYOFWEIRDFISHES_SROLLINGLISTSEVALUATOR_H

class sRollingListsEvaluator {
public:

    explicit sRollingListsEvaluator(int wSize) {
        for (int i = 0; i < wSize; i++)
            sRLE_winList.push_back(std::make_shared<std::pair<uint32_t, uint32_t>>());
    }

    double dropLastWindow() {
        std::pair<uint32_t, uint32_t> ret = *sRLE_winList.front();
        sRLE_winList.pop_front();
        sRLE_winList.push_back(std::make_shared<std::pair<uint32_t, uint32_t>>());
        return 1.0 + (0.1 * (double) ret.first) - (0.1 * (double) ret.second);
    }

    [[nodiscard]] double getDesirability() const {
        uint32_t nTotalCreated = 0;
        uint32_t nTotalSold = 0;
        for (const auto &p: sRLE_winList) {
            nTotalCreated += p->first;
            nTotalSold += p->first;
        }
        return 1.0 + (0.1 * (double) nTotalCreated) - (0.1 * (double) nTotalSold);
    }

    void addLastCreate() { sRLE_winList.back()->first++; }

    void addLastBought() { sRLE_winList.back()->second++; }

private:
    std::list<std::shared_ptr<std::pair<uint32_t, uint32_t>>> sRLE_winList;
};

#endif //CITYOFWEIRDFISHES_SROLLINGLISTSEVALUATOR_H
