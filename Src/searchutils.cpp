#include "searchutils.h"

bool searchutils::operator==(const searchutils::Point &lhs, const searchutils::Point &rhs) {
    return ((lhs.i == rhs.i) && (lhs.j == rhs.j));
}

searchutils::Point::Point(int i, int j) : i(i), j(j) {
}

std::size_t searchutils::PointHash::operator()(const searchutils::Point &point) const {
    return size_t(point.i) + (size_t(point.j) << 32u);
}

