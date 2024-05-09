//
// Created by ganymede on 5/8/24.
//

#ifndef CITYOFWEIRDFISHES_SGASTARROAD_H
#define CITYOFWEIRDFISHES_SGASTARROAD_H

#include "node/rNode.h"
#include "sgRoadsMain.h"

struct objAStarCar {
    rNode *oASC_dActNode;
    uint8_t oASC_nTimeAct;
    std::vector<uint8_t> oASC_dRoute;
    uint32_t oASC_dIndex;
};

class sgAStarRoad : sEventReceiver {
public:

    explicit sgAStarRoad(const std::shared_ptr<sgRoadsMain> &inSgRoadsMain) : sgASR_sgRoadsMain(inSgRoadsMain) {
        std::shared_ptr<sEventManager> sEM = sEventManager::getInstance();
        sEM->addReceiverToEvent(sEventManager::sEM_Events::sEM_EventSoftLockCar, this);
    }

    void tick() {
        for (objAStarCar &oASC: sgASR_ActiveRCars) {
            if (oASC.oASC_dActNode->refCompressed->isCrossing() ||
                oASC.oASC_nTimeAct >= oASC.oASC_dActNode->refCompressed->nCompressed) {

                oASC.oASC_dIndex++;
                rNode *rNode::* directions[] = {&rNode::rTop, &rNode::rRight, &rNode::rBottom, &rNode::rLeft};
                oASC.oASC_dActNode = oASC.oASC_dActNode->*directions[oASC.oASC_dRoute[oASC.oASC_dIndex]];

                oASC.oASC_nTimeAct = 0;
            } else
                oASC.oASC_nTimeAct++;
        }
    }

    void er_EventSoftLockCar(const std::pair<uint32_t, uint32_t> inStrPos,
                             const std::pair<uint32_t, uint32_t> inEndPos) override {
        addCarAsAStarRoute(inStrPos, inEndPos);
    }

private:


    void
    addCarAsAStarRoute(const std::pair<uint32_t, uint32_t> &inStrRoad, const std::pair<uint32_t, uint32_t> &inEndRoad) {
        rNode *startNode = sgASR_sgRoadsMain->gLayerRoads[inStrRoad.first][inStrRoad.second];
        sgASR_ActiveRCars.push_back(
                {startNode, 0, findRouteDirection(startNode, inEndRoad)});
    }

    static bool isValidMove(rNode *node) { return node != nullptr; }

    static float heuristic(const std::pair<uint32_t, uint32_t> &a, const std::pair<uint32_t, uint32_t> &b) {
        return (float) (std::abs((int) a.first - (int) b.first) + std::abs((int) a.second - (int) b.second));
    }

    static std::vector<rNode *>
    findRoute(rNode *startNode, const std::pair<uint32_t, uint32_t> &endPos) {
        std::vector<rNode *> route;
        std::priority_queue<std::pair<float, rNode *>, std::vector<std::pair<float, rNode *>>, std::greater<>> pq;
        std::unordered_set<rNode *> visited;
        std::unordered_map<rNode *, float> gScore;
        std::unordered_map<rNode *, rNode *> cameFrom;

        pq.emplace(0.0f, startNode);
        gScore[startNode] = 0.0f;

        while (!pq.empty()) {
            rNode *current = pq.top().second;
            pq.pop();

            if (current->rPos == endPos) {
                // Destination reached, reconstruct the path
                while (current != nullptr) {
                    route.insert(route.begin(), current); //que vagi per direccio
                    current = cameFrom[current];
                }
                return route;
            }

            visited.insert(current);

            // Explore valid neighboring nodes
            std::vector<rNode *> neighbors = {current->rLeft, current->rRight, current->rTop, current->rBottom};
            for (rNode *neighbor: neighbors) {
                if (isValidMove(neighbor) && visited.find(neighbor) == visited.end()) {
                    float tentative_gScore = gScore[current] + 1.0f; // Assuming unit cost for each move
                    if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
                        cameFrom[neighbor] = current;
                        gScore[neighbor] = tentative_gScore;
                        float fScore = tentative_gScore + heuristic(neighbor->rPos, endPos);
                        pq.emplace(fScore, neighbor);
                    }
                }
            }
        }

        // If no route is found, return an empty vector
        return route;
    }

    static std::vector<uint8_t> findRouteDirection(rNode *startNode, const std::pair<uint32_t, uint32_t> &endPos) {
        std::vector<rNode *> _rRoad = findRoute(startNode, endPos);
        std::vector<uint8_t> _retVar(_rRoad.size());
        _retVar[0] = 4;
        for (int i = 1; i < _rRoad.size(); i++) {
            if (_rRoad[i - 1]->rTop == _rRoad[i])
                _retVar[i] = 0;
            else if (_rRoad[i - 1]->rRight == _rRoad[i])
                _retVar[i] = 1;
            else if (_rRoad[i - 1]->rBottom == _rRoad[i])
                _retVar[i] = 2;
            else if (_rRoad[i - 1]->rLeft == _rRoad[i])
                _retVar[i] = 3;
        }
        return _retVar;
    }

    std::shared_ptr<sgRoadsMain> sgASR_sgRoadsMain;
    std::list<objAStarCar> sgASR_ActiveRCars;
};

#endif //CITYOFWEIRDFISHES_SGASTARROAD_H
