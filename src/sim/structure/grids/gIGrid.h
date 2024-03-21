//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GIGRID_H
#define CITYOFWEIRDFISHES_GIGRID_H

#include <utility>
#include <iostream>
#include <string>

template<typename T>
class gIGrid {
public:
    virtual ~gIGrid() = default;

    virtual void set(int row, int col, const T &value) = 0;

    void set(const std::pair<int, int> p, const T &value) { return set(p.first, p.second, value); }

    virtual T get(int row, int col) const = 0;

    T get(const std::pair<int, int> p) const { return get(p.first, p.second); }

    [[nodiscard]] virtual std::pair<std::pair<int, int>, std::pair<int, int>> rangeUse() const = 0;

    [[nodiscard]] virtual bool isInside(const std::pair<int, int> &p) const { return isInside(p.first, p.second); }

    [[nodiscard]] virtual bool isInside(const int &pX, const int &pY) const = 0;

};

#endif //CITYOFWEIRDFISHES_GIGRID_H
