#include "search.h"
#include "node.h"
#include <algorithm>
#include<chrono>
#include<iterator>
#include "gl_const.h"

Search::Search() {
//set defaults here
}

Search::~Search() {}

double calculateHeuristic(int pos_i, int pos_j, int goal_i, int goal_j, int metrictype) {
    int d_i = std::abs(pos_i - goal_i);
    int d_j = std::abs(pos_j - goal_j);

    if (metrictype == CN_SP_MT_DIAG) {
        return abs(d_i - d_j) + sqrt(2) * std::min(d_i, d_j);
    }

    if (metrictype == CN_SP_MT_MANH) {
        return d_i + d_j;
    }

    if (metrictype == CN_SP_MT_EUCL) {
        return sqrt(d_i * d_i + d_j * d_j);
    }

    if (metrictype == CN_SP_MT_CHEB) {
        return std::max(d_i, d_j);
    }

    return 0;
}

Node buildNode(Node *parentNode, int cur_i, int cur_j, const Map &map, const EnvironmentOptions &options) {
    Node new_node{};
    new_node.i = cur_i;
    new_node.j = cur_j;
    new_node.parent = parentNode;
    double dist_from_cur_to_new = ((cur_i == parentNode->i || cur_j == parentNode->j) ? 1 : sqrt(2));
    new_node.g = parentNode->g + dist_from_cur_to_new;
    new_node.H = calculateHeuristic(
            new_node.i, new_node.j, map.getGoal_i(), map.getGoal_j(), options.metrictype);
    new_node.F = new_node.g + new_node.F;
    return new_node;
}

// current version does not support options
std::vector<Node> getSucessors(Node *parentNode, const Map &map, const EnvironmentOptions &options) {
    std::vector<Node> sucessors;
    for (int i = parentNode->i - 1; i <= parentNode->i + 1; i++) {
        for (int j = parentNode->j - 1; j <= parentNode->j + 1; j++) {
            if ((i == parentNode->i && j == parentNode->j) || !map.CellOnGrid(i, j) ||
                map.CellIsObstacle(i, j))
                continue;
            sucessors.push_back(buildNode(parentNode, i, j, map, options));
        }
    }
    return sucessors;
}

std::list<Node> generate_lppath(const Node *node) {
    std::list<Node> lppath;
    while (node != nullptr) {
        lppath.push_front(*node);
        node = node->parent;
    }
    return lppath;
}

std::list<Node> generate_hppath_from_lppath(const std::list<Node> &lppath) {
    if (lppath.size() <= 2) {
        return lppath;
    }
    std::list<Node> hppath;
    hppath.push_back(lppath.front());

    for (auto cur = std::next(lppath.begin(), 2); cur != lppath.end(); cur++) {
        auto prev = std::prev(cur, 1);
        auto pprev = std::prev(cur, 2);
        if (!((cur->i - prev->i == prev->i - pprev->i) && (cur->j - prev->j == prev->j - pprev->j))) {
            hppath.push_back(*prev);
        }
    }
    hppath.push_back(*(--lppath.end()));
    return hppath;
}

SearchResult Search::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options) {
    auto start_time = std::chrono::steady_clock::now();

    double start_node_heuristic = calculateHeuristic(map.getStart_i(), map.getStart_j(), map.getGoal_j(),
                                                     map.getGoal_j(), options.metrictype);
    open.push_back(Node{map.getStart_i(), map.getStart_j(), start_node_heuristic, 0, start_node_heuristic, nullptr});
    Node *goal_node = nullptr;

    int numberofsteps = 0;

    while (!open.empty()) {
        numberofsteps++;
        auto expanding_node_it = std::min_element(open.begin(), open.end(), [](const Node &a, const Node &b) {
            return a.F < b.F;
        });

        close.push_back(*expanding_node_it);
        open.erase(expanding_node_it);
        expanding_node_it = --close.end();

        if (expanding_node_it->i == map.getGoal_i() && expanding_node_it->j == map.getGoal_j()) {
            goal_node = &(*expanding_node_it);
            break;
        }

        std::vector<Node> sucessors = getSucessors(&(*expanding_node_it), map, options);
        for (const Node &s : sucessors) {
            if (std::find_if(close.begin(), close.end(), [&s](const Node &node) {
                return (node.i == s.i) && (node.j == s.j);
            }) != close.end()) {
                continue;
            }

            auto it = std::find_if(open.begin(), open.end(), [&s](const Node &node) {
                return (node.i == s.i) && (node.j == s.j);
            });
            if (it == open.end()) {
                open.push_back(s);
            } else {
                if (it->F > s.F) {
                    open.erase(it);
                    open.push_back(s);
                }
            }
        }
    }

    sresult.pathfound = (goal_node != nullptr);
    lppath = generate_lppath(goal_node);
    hppath = generate_hppath_from_lppath(lppath);

    sresult.lppath = &lppath;
    sresult.hppath = &hppath;
    sresult.nodescreated = open.size() + close.size();
    sresult.numberofsteps = numberofsteps;
    sresult.pathlength = float(goal_node->g);
    sresult.time = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - start_time).count();
    return sresult;
    /*sresult.pathfound = ;
    sresult.nodescreated =  ;
    sresult.numberofsteps = ;
    sresult.time = ;
    sresult.hppath = &hppath; //Here is a constant pointer
    sresult.lppath = &lppath;*/
}

/*void Search::makePrimaryPath(Node curNode)
{
    //need to implement
}*/

/*void Search::makeSecondaryPath()
{
    //need to implement
}*/
