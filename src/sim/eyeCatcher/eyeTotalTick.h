//
// Created by ganymede on 4/9/24.
//

#ifndef CITYOFWEIRDFISHES_EYETOTALTICK_H
#define CITYOFWEIRDFISHES_EYETOTALTICK_H

class eyeTotalTick {
public:
    static void pTick() { eTT_Tick++; }

    static int getTick() { return eTT_Tick; }

private:
    static int eTT_Tick;
};

int  eyeTotalTick::eTT_Tick = 0l;
#endif //CITYOFWEIRDFISHES_EYETOTALTICK_H
