//
// Created by ganymede on 3/22/24.
//

#ifndef CITYOFWEIRDFISHES_SROLLINGLISTSEVALUATOR_H
#define CITYOFWEIRDFISHES_SROLLINGLISTSEVALUATOR_H

/**
 * @class sRollingListsEvaluator
 * @brief This class is responsible to keep track of the last n days how many creations and selling of a product have been made
 */
class sRollingListsEvaluator {
public:

    explicit sRollingListsEvaluator(int wSize) {
        for (int i = 0; i < wSize; i++)
            sRLE_winList.push_back(std::make_shared<std::pair<uint32_t, uint32_t>>());
    }

    /**
     * @fn double dropLastWindow
     * @brief Drop the last window of the class and return the desirability computed given that window. And creates a new one
     * @return The desirability computed using the dropped window
     */
    [[nodiscard]] double dropLastWindow() {
        std::pair<uint32_t, uint32_t> ret = *sRLE_winList.front();
        sRLE_winList.pop_front();
        sRLE_winList.push_back(std::make_shared<std::pair<uint32_t, uint32_t>>());
        return  _calcDesirability(ret);
    }

    /**
     * @fn double getDesirability
     * @return Return the desirability computed using all the windows of creations and purchases
     */
    [[nodiscard]] double getDesirability() const {
        uint32_t nTotalCreated = 0;
        uint32_t nTotalSold = 0;
        for (const auto &p: sRLE_winList) {
            nTotalCreated += p->first;
            nTotalSold += p->second;
        }
        return _calcDesirability({nTotalSold, nTotalCreated});
    }

    /**
     * @fn void addLastCreate
     * @brief Add in the first window a count in the number of creations of that product
     */
    void addLastCreate() { sRLE_winList.back()->first++; }

    /**
     * @fn void addLastBought
     * @brief Add in the first window a count in the number of purchases of that product
     */
    void addLastBought() { sRLE_winList.back()->second++; }

private:

    /**
     * @fn double _calcDesirability
     * @brief Given the number of purchases and creations, compute the total level of desirability for that product
     * @param inPTotal A pair containing positive numbers of creations and purchases for a given product
     * @return A number that represents the desirability, 1.0 its basic, less is negative, more than 1 represents more desirability
     */
    static double _calcDesirability(const std::pair<int,int>& inPTotal){
        return 1.0 - (0.01 * (double) inPTotal.first) + (0.01 * (double) inPTotal.second);
    }
    std::list<std::shared_ptr<std::pair<uint32_t, uint32_t>>> sRLE_winList;
};

#endif //CITYOFWEIRDFISHES_SROLLINGLISTSEVALUATOR_H
