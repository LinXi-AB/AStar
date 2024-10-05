#include "point.h"

Point::Point(const Point &other) : line_number(other.line_number), row_number(other.row_number),
    type(other.type), total_cost(other.total_cost),
    current_cost(other.current_cost), pre(other.pre) {}

Point &Point::operator=(const Point &point) {
    if (this != &point) { // 自我赋值检查
        line_number = point.line_number;
        row_number = point.row_number;
        type = point.type;
        total_cost = point.total_cost;
        current_cost = point.current_cost;
        pre = point.pre; // 浅拷贝，假设 pre 的所有权不变
    }
    return *this;
}
