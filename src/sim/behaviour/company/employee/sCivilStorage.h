//
// Created by ganymede on 4/8/24.
//

#ifndef CITYOFWEIRDFISHES_SCIVILSTORAGE_H
#define CITYOFWEIRDFISHES_SCIVILSTORAGE_H

#include <map>
#include <list>
#include "../../../structure/obj/elements/objCivil.h"
#include "../../iVectorStorage.h"

class rVectorCivil : public iVectorStorage<objCivil> {
public:

    explicit rVectorCivil(uint32_t nMaxCivil) : iVectorStorage(nMaxCivil) {};
};

class sCivilStorage {
public:
    sCivilStorage() : sCS_vecCivil(std::make_shared<rVectorCivil>(30000)) {}

    uint32_t storeCivil(const std::shared_ptr<objCivil> &inNewCivil, uint32_t inUuidCompany,
                        std::pair<std::list<objCivil>::iterator, std::list<objCivil>::iterator> inRoute) {
        uint32_t _uuidCivil = sCS_vecCivil->storeElement(inNewCivil);

        if (sCS_mapCivilByCompany.find(inUuidCompany) == sCS_mapCivilByCompany.end())
            sCS_mapCivilByCompany[inUuidCompany] = {};

        sCS_mapCivilByCompany[inUuidCompany].push_back({_uuidCivil, inRoute.first, inRoute.second});
        return _uuidCivil;
    }
    [[nodiscard]] std::shared_ptr<objCivil> getByUuid(uint32_t uuidCivil) const{
        return sCS_vecCivil->getElementByUuid(uuidCivil);
    }

    struct sCS_cCivRoute {
        uint32_t sCS_cCRCivil;
        std::list<objCivil>::iterator sCS_cCRUrBegin;
        std::list<objCivil>::iterator sCS_cCRUrEnd;
    };

    sCS_cCivRoute routeCivilGivenCompany(uint32_t uuidCompany){
        return sCS_mapCivilByCompany[uuidCompany].front();
    }

    void removeCivilGivenCompany(uint32_t uuidCompany){
        sCS_cCivRoute _cCR = sCS_mapCivilByCompany[uuidCompany].front();
        sCS_mapCivilByCompany[uuidCompany].pop_front();
        sCS_vecCivil->removeElement(_cCR.sCS_cCRCivil);
    }

private:
    std::map<uint32_t, std::list<sCS_cCivRoute>> sCS_mapCivilByCompany;
    std::shared_ptr<rVectorCivil> sCS_vecCivil;
};

#endif //CITYOFWEIRDFISHES_SCIVILSTORAGE_H
