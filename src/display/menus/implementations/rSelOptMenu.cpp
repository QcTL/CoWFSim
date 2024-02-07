//
// Created by Laminar on 31/01/2024.
//

#include "rSelOptMenu.h"
#include "../../../IO/ReaderParameters.h"
#include "../../../common/RelPath.h"


rSelOptMenu::rSelOptMenu(const std::string &pthFileD, rIMenu::rRelativePos rPos):rIMenu(rPos) {
    std::map<std::string, std::string> sm = ReaderParameters::readFile(
            (RelPath::relPath / "files" / "graphic" / "menus" / (pthFileD + R"(.txt)")).string());

    std::ifstream file((RelPath::relPath / "files" / "graphic" / "menus" / sm["src"]).string());

    std::string line;
    std::vector<std::vector<int>> data;

    while (std::getline(file, line)) {
        std::vector<int> row;
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            row.push_back(value);
        }
        data.push_back(row);
    }

    for (int i = 0; i < data.size(); ++i)
        for (int j = 0; j < data[i].size(); ++j)
            if (data[i][j] == 304 || data[i][j] == 305)
                switch (rPos) {

                    case pBottomLeft:
                        pElemSel.emplace_back(data.size()-1-i, j);
                        break;
                    case pBottomRight:
                        pElemSel.emplace_back(data.size()-1-i,  data[i].size()-1-j);
                        break;
                    case pTopLeft:
                        pElemSel.emplace_back(i, j);
                        break;
                    case pTopRight:
                        pElemSel.emplace_back(i,  data[i].size()-1-j);
                        break;
                };

    file.close();
    dInfo = getVertexMenu(data[0].size(),data.size(),data);
    gWidth = data[0].size();
}

void rSelOptMenu::draw(sf::RenderWindow &rW) {
    rW.draw(dInfo, &tsTex.tsTex);
}

void rSelOptMenu::setNewSel(int v) {
    sf::Vertex* quad = &dInfo[(pElemSel[cCurrenSel].second + pElemSel[cCurrenSel].first * gWidth) * 4];
    for (int k = 0; k < 4; k++) {
        quad[k].texCoords = lRefTiles[305][k];
    }

    quad = &dInfo[(pElemSel[v].second + pElemSel[v].first * gWidth) * 4];
    for (int k = 0; k < 4; k++) {
        quad[k].texCoords = lRefTiles[304][k];
    }

    cCurrenSel = v;
}
