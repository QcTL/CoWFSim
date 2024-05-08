//
// Created by ganymede on 4/8/24.
//

#ifndef CITYOFWEIRDFISHES_SCIVILSTORAGE_H
#define CITYOFWEIRDFISHES_SCIVILSTORAGE_H

#include <map>
#include <list>
#include "../../../structure/obj/elements/objCivil.h"
#include "../../iVectorStorage.h"

/**
 * @class rVectorCivil
 * @brief An implementation of iVectorStorage to store @ref objCivil
 */
class rVectorCivil : public iVectorStorage<objCivil> {
public:

    explicit rVectorCivil(uint32_t nMaxCivil) : iVectorStorage(nMaxCivil) {};
};

/**
 * @class sCivilStorage
 * @brief This class is used to interact with the storage of the objCivils elements
 */
class sCivilStorage {
public:
    sCivilStorage() : sCS_vecCivil(std::make_shared<rVectorCivil>(30000)) {}

    uint32_t storeCivil(const std::shared_ptr<objCivil> &inNewCivil, uint32_t inUuidCompany,
                        uint32_t inRoute) {
        uint32_t _uuidCivil = sCS_vecCivil->addElement(inNewCivil);

        if (sCS_mapCivilByCompany.find(inUuidCompany) == sCS_mapCivilByCompany.end())
            sCS_mapCivilByCompany[inUuidCompany] = {};

        sCS_mapCivilByCompany[inUuidCompany].push_back({_uuidCivil, inRoute});
        return _uuidCivil;
    }

    /**
     * @fn std::shared_ptr<objCivil> getByUuid
     * @brief Gives a pointer to a objCivil referenced by the their uuid
     * @param uuidCivil A valid uuid that represents a objCivil at some point created
     * @return A pointer to the represented objCivil
     */
    [[nodiscard]] std::shared_ptr<objCivil> getByUuid(uint32_t uuidCivil) const{
        return sCS_vecCivil->getElementByUuid(uuidCivil);
    }

    struct sCS_cCivRoute {
        uint32_t sCS_cCRCivil;
        uint32_t sCS_cCRRouteUuid;
    };

    /**
     * @fn sCS_cCivRoute routeCivilGivenCompany
     * @param uuidCompany The uuid of the company you want to get some route from, it has to be valid and come from some
     * company valid and created
     * @return A route having that company as an end position
     */
    sCS_cCivRoute routeCivilGivenCompany(uint32_t uuidCompany){
        return sCS_mapCivilByCompany[uuidCompany].front();
    }

    /**
     * @fn void removeCivilGivenCompany
     * @brief Remove the civil that has the longest time working in the company
     * @param uuidCompany The valid uuid of a company, that has at some point being created, that you want to remove a civilian
     */
    void removeCivilGivenCompany(uint32_t uuidCompany){
        sCS_cCivRoute _cCR = sCS_mapCivilByCompany[uuidCompany].front();
        sCS_mapCivilByCompany[uuidCompany].pop_front();
        sCS_vecCivil->removeElementByUuid(_cCR.sCS_cCRCivil);
    }

private:
    std::map<uint32_t, std::list<sCS_cCivRoute>> sCS_mapCivilByCompany;
    std::shared_ptr<rVectorCivil> sCS_vecCivil;
};

#endif //CITYOFWEIRDFISHES_SCIVILSTORAGE_H
