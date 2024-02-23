//
// Created by ganymede on 2/23/24.
//

#ifndef CITYOFWEIRDFISHES_RREMOTEUPDATEGRID_H
#define CITYOFWEIRDFISHES_RREMOTEUPDATEGRID_H
class rRemoteUpdateGrid{
public:
    static void setHasToChange(const bool &newVal){
        hasToUpdate =  newVal;
    }

    static bool hasToUpdate;
};
inline bool rRemoteUpdateGrid::hasToUpdate;


#endif //CITYOFWEIRDFISHES_RREMOTEUPDATEGRID_H
