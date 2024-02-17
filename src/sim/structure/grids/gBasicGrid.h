//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GBASICGRID_H
#define CITYOFWEIRDFISHES_GBASICGRID_H

#include <vector>
#include <iostream>
#include <string>
#include "gIGrid.h"
#include <iomanip>

template<typename T>
class gBasicGrid : public gIGrid<T> {

public:
    gBasicGrid(int rows, int cols) : data_(rows, std::vector<T>(cols)) {}

    gBasicGrid(int rows, int cols, T defValue) : data_(rows, std::vector<T>(cols, defValue)) {}

    void set(int row, int col, const T &value) override {
        if (isInside(row, col)) {
            data_[row][col] = value;
        } else {
            std::cout << "WARNING: A SET OUTSIDE OF THE GRID HAS BEEN CALLED" << std::endl;
        }
    }

    T get(int row, int col) override {
        if (isInside(row, col)) {
            return data_[row][col];
        } else {
            std::cout << "WARNING: A GET OUTSIDE OF THE GRID HAS BEEN CALLED" << std::endl;
            return data_[0][0];
        }
    }

    [[nodiscard]] std::pair<std::pair<int, int>, std::pair<int, int>> rangeUse() const override {
        //X, Y
        return {{0, data_.empty() ? 0 : data_[0].size() - 1},
                {0, data_.size() - 1}};
    }

    bool isInside(std::pair<int, int> p) override {
        return 0 <= p.first && p.first < data_.size() && 0 <= p.second && p.second < data_[0].size();
    }

    bool isInside(int pX, int pY) override {
        return 0 <= pX && pX < data_.size() && 0 <= pY && pY < data_[0].size();
    }

    friend std::ostream &operator<<(std::ostream &os, const gBasicGrid &obj) {
        // Determine the maximum width.
        int max_width = 0;
        for (const auto &row: obj.data_) {
            for (const auto &elem: row) {
                max_width = std::max(max_width, static_cast<int>(std::to_string(elem).length()));
            }
        }

        // Print the data.
        for (const auto &row: obj.data_) {
            for (const auto &elem: row) {
                os << std::setw(max_width) << elem << " ";
            }
            os << "\n";
        }
        return os;
    }

private:
    std::vector<std::vector<T>> data_;
};

#endif //CITYOFWEIRDFISHES_GBASICGRID_H
