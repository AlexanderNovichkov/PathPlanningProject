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

double Search::calculateHeuristic(int pos_i, int pos_j, int goal_i, int goal_j, int metrictype) {
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

    if (metrictype == CN_SP_MT_ZERO) {
        return 0;
    }

    return 0;
}

Node Search::buildNode(Node *parentNode, int cur_i, int cur_j, const Map &map, const EnvironmentOptions &options) {
    Node new_node{};
    new_node.i = cur_i;
    new_node.j = cur_j;
    new_node.parent = parentNode;
    double dist_from_cur_to_new = ((cur_i == parentNode->i || cur_j == parentNode->j) ? 1 : CN_SQRT_TWO);
    new_node.g = parentNode->g + dist_from_cur_to_new;
    new_node.H = calculateHeuristic(
            new_node.i, new_node.j, map.getGoal_i(), map.getGoal_j(), options.metrictype);
    new_node.F = new_node.g + options.hweight * new_node.H;
    return new_node;
}


std::vector<Node> Search::getSucessors(Node *parentNode, const Map &map, const EnvironmentOptions &options) {
    std::vector<Node> sucessors;
    for (int i = parentNode->i - 1; i <= parentNode->i + 1; i++) {
        for (int j = parentNode->j - 1; j <= parentNode->j + 1; j++) {
            if ((i == parentNode->i && j == parentNode->j) || !map.CellOnGrid(i, j) || map.CellIsObstacle(i, j)) {
                continue;
            }


            if (i != parentNode->i && j != parentNode->j) { // move diagonal
                if (!options.allowdiagonal) {
                    continue;
                }

                int untraversable_adjacent_cell_cnt = 0;
                untraversable_adjacent_cell_cnt += (map.CellOnGrid(i, parentNode->j) &&
                                                    map.CellIsObstacle(i, parentNode->j));

                untraversable_adjacent_cell_cnt += (map.CellOnGrid(parentNode->i, j) &&
                                                    map.CellIsObstacle(parentNode->i, j));

                if (untraversable_adjacent_cell_cnt >= 1) {
                    if (!options.cutcorners) {
                        continue;
                    }

                    if (untraversable_adjacent_cell_cnt == 2) {
                        if (!options.allowsqueeze) {
                            continue;
                        }
                    }
                }
            }

            sucessors.push_back(buildNode(parentNode, i, j, map, options));
        }
    }

    return sucessors;
}

SearchResult Search::startSearch(ILogger *Logger, const Map &map, const EnvironmentOptions &options) {
    auto start_time = std::chrono::steady_clock::now();
    double start_node_heuristic = calculateHeuristic(map.getStart_i(), map.getStart_j(), map.getGoal_j(),
                                                     map.getGoal_j(), options.metrictype);

    searchutils::OpenDataStructure open(options.breakingties);
    std::unordered_map<uint64_t, Node> close;

    open.update_node(Node{map.getStart_i(), map.getStart_j(), start_node_heuristic, 0, start_node_heuristic, nullptr});
    Node *goal_node = nullptr;


    while (open.size() > 0) {
        Node expanding_node = open.extract_min_node();
        close[searchutils::point_hash(expanding_node.i, expanding_node.j)] = expanding_node;

        Node *cur_node_ptr = &close[searchutils::point_hash(expanding_node.i, expanding_node.j)];

        if (cur_node_ptr->i == map.getGoal_i() && cur_node_ptr->j == map.getGoal_j()) {
            goal_node = cur_node_ptr;
            break;
        }

        std::vector<Node> sucessors = getSucessors(cur_node_ptr, map, options);
        for (const Node &s : sucessors) {
            if (close.count(searchutils::point_hash(s.i, s.j))) {
                continue;
            }

            open.update_node(s);
        }
    }

    sresult.pathfound = (goal_node != nullptr);
    makePrimaryPath(goal_node);
    makeSecondaryPath();

    sresult.lppath = &lppath;
    sresult.hppath = &hppath;
    sresult.nodescreated = open.size() + close.size();
    sresult.numberofsteps = (int) (close.size()) - 1;
    sresult.pathlength = float(goal_node->g);
    sresult.time = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now() - start_time).count() / 1000000.0;

    return sresult;
}

void Search::makePrimaryPath(const Node *node) {
    while (node != nullptr) {
        lppath.push_front(*node);
        node = node->parent;
    }
}

void Search::makeSecondaryPath() {
    if (lppath.size() <= 2) {
        return;
    }
    hppath.push_back(lppath.front());

    for (auto cur = std::next(lppath.begin(), 2); cur != lppath.end(); cur++) {
        auto prev = std::prev(cur, 1);
        auto pprev = std::prev(cur, 2);
        if (!((cur->i - prev->i == prev->i - pprev->i) && (cur->j - prev->j == prev->j - pprev->j))) {
            hppath.push_back(*prev);
        }
    }
    hppath.push_back(*(--lppath.end()));
}
