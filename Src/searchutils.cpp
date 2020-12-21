#include "searchutils.h"

namespace searchutils {

    uint64_t point_hash(int i, int j) {
        return (uint64_t) (i) | (uint64_t(j) << 32u);
    }


    OpenDataStructure::OpenDataStructure() = default;

    void OpenDataStructure::update_node(const Node &node) {
        uint64_t pos_hash = point_hash(node.i, node.j);

        auto it = ceil_to_node.find(pos_hash);
        if (it != ceil_to_node.end()) {
            if (node.F < it->second.F) {
                sorted_nodes.erase(&(it->second));
                it->second = node;
                sorted_nodes.insert(&(it->second));
            }
        } else {
            ceil_to_node[pos_hash] = node;
            sorted_nodes.insert(&ceil_to_node[pos_hash]);
        }
    }

    Node OpenDataStructure::extract_min_node() {
        Node *nodeptr = *sorted_nodes.begin();
        sorted_nodes.erase(sorted_nodes.begin());

        Node node = ceil_to_node[point_hash(nodeptr->i, nodeptr->j)];
        ceil_to_node.erase(point_hash(nodeptr->i, nodeptr->j));
        return node;
    }

    size_t OpenDataStructure::size() const {
        return sorted_nodes.size();
    }

    void OpenDataStructure::set_breakingties(bool value) {
        breakingties = value;
    }

    bool OpenDataStructure::comp::operator()(const Node *a, const Node *b) {
        if (a->F != b->F) {
            return a->F < b->F;
        }
        ////// need to change
        if (a->g != b->g) {
            return a->g > b->g;
        }
        ///////
        if (a->i != b->i) {
            return a->i < b->i;
        }
        return (a->j < b->j);
    }
}