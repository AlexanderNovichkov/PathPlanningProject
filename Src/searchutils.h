#ifndef ALEXANDER_NOVICHKOV_ASEARCH_SEARCHUTILS_H
#define ALEXANDER_NOVICHKOV_ASEARCH_SEARCHUTILS_H

#include "node.h"
#include "environmentoptions.h"
#include <algorithm>
#include<unordered_map>
#include<set>


namespace searchutils {
    uint64_t point_hash(int i, int j);

    struct Comp {
        Comp(bool breakingties);
        bool breakingties;
        bool operator()(const Node *a, const Node *b) const;
    };

    class OpenDataStructure {
    public:
        OpenDataStructure(bool breakingties);


        void update_node(const Node &node);

        Node extract_min_node();

        size_t size() const;

    private:
        std::set<Node *, Comp> sorted_nodes;
        std::unordered_map<uint64_t, Node> ceil_to_node;
    };
}


#endif //ALEXANDER_NOVICHKOV_ASEARCH_SEARCHUTILS_H
