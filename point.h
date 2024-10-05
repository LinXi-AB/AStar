#ifndef POINT_H
#define POINT_H

#include <QWidget>

class Point : public QWidget
{
    Q_OBJECT
public:
    // 点的类型
    enum PointType {
        Path,
        Wall,
        Origin,
        Terminal,
        Visiting,
        Visited,
        Surround,
        ShortestPath
    };

    Point() {};
    Point(const int& line, const int& row) :line_number(line), row_number(row) {};
    // 拷贝构造函数与=重载
    Point(const Point& other);
    Point& operator=(const Point& point);

public:
    int line_number = -1;
    int row_number = -1;
    int type = 0;
    int total_cost = 0;
    int current_cost = 0;
    Point* pre = nullptr;

signals:

};

#endif // POINT_H
