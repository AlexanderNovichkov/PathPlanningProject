#ifndef ALEXANDER_NOVICHKOV_ASEARCH_SEARCHUTILS_H
#define ALEXANDER_NOVICHKOV_ASEARCH_SEARCHUTILS_H

#include "node.h"
#include <algorithm>

namespace searchutils {
    struct Point {
        int i, j;

        Point(int i, int j);

        friend bool operator==(const Point &lhs, const Point &rhs);
    };

    struct PointHash{
        std::size_t operator()(const Point &point) const;
    };



}


#endif //ALEXANDER_NOVICHKOV_ASEARCH_SEARCHUTILS_H
