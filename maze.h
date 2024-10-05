#ifndef MAZE_H
#define MAZE_H

#include <QWidget>
#include "point.h"
#include <vector>
#include <queue>
#include <QPainter>
#include <QTimer>

// 用于优先队列中比较点的代价
class SmallerCost {
public:
    bool operator()(const Point& point1, const Point& point2) {
        return point1.total_cost > point2.total_cost;
    }
};

class Maze : public QWidget
{
    Q_OBJECT
public:
    explicit Maze(const std::vector<std::vector<Point>>& map, int width, int height, QWidget *parent = nullptr);

    enum SearchStatus {
        Searching,
        Success,
        Failed
    };

    bool is_invalid_position(int line, int row, const int& dline, const int& drow);
    bool is_terminal(const Point& point);
    int calc_total_cost(const Point& point);
    void search_shortest_path();
    bool AStar();
    void draw(QPainter* painter);
    int AStarSearchOnce();
    void refresh();

private:
    int width = 50;
    int height = 50;
    int line_number = 10;
    int row_number = 10;
    std::vector<std::vector<Point>> map;
    Point origin;
    Point terminal;
    std::priority_queue<Point, std::vector<Point>, SmallerCost> AStar_queue;// 优先队列
    bool is_AStar_started = false;// 是否执行过AStar

signals:

};

#endif // MAZE_H
