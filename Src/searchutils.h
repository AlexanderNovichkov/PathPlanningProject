#ifndef ALEXANDER_NOVICHKOV_ASEARCH_SEARCHUTILS_H
#define ALEXANDER_NOVICHKOV_ASEARCH_SEARCHUTILS_H

#include "node.h"
#include "environmentoptions.h"
#include <algorithm>
#include<unordered_map>
#include<set>


namespace searchutils {
    uint64_t point_hash(int i, int j);


    class OpenDataStructure {
    public:
        OpenDataStructure();

        void set_breakingties(bool value);

        void update_node(const Node &node);

        Node extract_min_node();

        size_t size() const;

    private:
        struct comp {
            bool operator()(const Node *a, const Node *b);
        };

    private:
        bool breakingties;
        std::set<Node *, comp> sorted_nodes;
        std::unordered_map<uint64_t, Node> ceil_to_node;
    };
}


#endif //ALEXANDER_NOVICHKOV_ASEARCH_SEARCHUTILS_H
