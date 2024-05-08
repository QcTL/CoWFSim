//
// Created by ganymede on 4/9/24.
//

#ifndef CITYOFWEIRDFISHES_EYETOTALTICK_H
#define CITYOFWEIRDFISHES_EYETOTALTICK_H

/**
 * @class eyeTotalTick
 * @brief Static class that keep track of the current timestamp that will be send with the eyesValue.
 *
 * Updated once per tick this class will set the timestamp that the @ref eyeValue will have when send outside the application
 * to be processed
 */
class eyeTotalTick {
public:

    /**
     * @fn pTick
     * @brief Static function that increments the tick count by one
     */
    static void pTick() { eTT_Tick++; }

    /**
     * @fn getTick
     * @brief Static function to obtain the current tick for @ref eyeValue reference
     * @return Int, number of current tick
     **/
    static int getTick() { return eTT_Tick; }

private:
    static int eTT_Tick;
};

int  eyeTotalTick::eTT_Tick = 0l;
#endif //CITYOFWEIRDFISHES_EYETOTALTICK_H
