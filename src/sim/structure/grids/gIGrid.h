//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GIGRID_H
#define CITYOFWEIRDFISHES_GIGRID_H

#include <utility>
#include <iostream>
#include <string>

/**
 * @class gIGrid
 * @brief This class is a interface to develop systems of grids accessed by a pair of coordinates
 * @tparam T The type of elements you want to have in each position of the grid
 */
template<typename T>
class gIGrid {
public:
    virtual ~gIGrid() = default;

    /**
     * @fn void set
     * @param row The row position that will be accessed, has to be "bool isInside" = True
     * @param col The col position that will be accessed, has to be "bool isInside" = True
     * @param value The value of type T that you want that position to have
     */
    virtual void set(int row, int col, const T &value) = 0;

    /**
     * @fn  void set
     * @param p A pair with the coordinates that you want to be accessed, has to be "bool isInside" = True
     * @param value The value of type T that you want that position to have
     */
    void set(const std::pair<int, int> p, const T &value) { return set(p.first, p.second, value); }

    /**
     * @fn T get
     * @param row The row position that will be accessed, has to be "bool isInside" = True
     * @param col The col position that will be accessed, has to be "bool isInside" = True
     * @return The value of T in that position of the grid
     */
    virtual T get(int row, int col) const = 0;

    /**
     * @fn  T get
     * @param p A pair with the coordinates that you want to be accessed, has to be "bool isInside" = True
     * @return The value of T in that position of the grid
     */
    T get(const std::pair<int, int> p) const { return get(p.first, p.second); }

    /**
     * @fn std::pair<std::pair<int, int>, std::pair<int, int>> rangeUse
     * @return The range in the x and y axis that the grid occupy
     */
    [[nodiscard]] virtual std::pair<std::pair<int, int>, std::pair<int, int>> rangeUse() const = 0;

    /**
     * @fn bool isInside
     * @param p A pair with the coordinates that you want to be accessed, has to be "bool isInside" = True
     * @return True if the point is inside the grid, false otherwise
     */
    [[nodiscard]] virtual bool isInside(const std::pair<int, int> &p) const { return isInside(p.first, p.second); }

    /**
     * @fn bool isInside
     * @param row The row position that will be accessed, has to be "bool isInside" = True
     * @param col The col position that will be accessed, has to be "bool isInside" = True
     * @return True if the point is inside the grid, false otherwise
     */
    [[nodiscard]] virtual bool isInside(const int &pX, const int &pY) const = 0;

};

#endif //CITYOFWEIRDFISHES_GIGRID_H
