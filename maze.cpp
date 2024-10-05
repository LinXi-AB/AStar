#include "maze.h"
#include <QPainter>
#include <QBrush>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include <QRandomGenerator>

Maze::Maze(const std::vector<std::vector<Point> > &map, int width, int height, QWidget *parent)
    : map(map), width(width), height(height), line_number(map.size()), row_number(map[0].size()), QWidget(parent) {
    for (int i = 0; i < line_number; i++) {
        for (int j = 0; j < row_number; j++) {
            // 地图初始化，给其中的每个点赋值line_number和row_number
            this->map[i][j].line_number = i;
            this->map[i][j].row_number = j;
            // 寻找终点和起点
            if (this->map[i][j].type == Point::PointType::Origin) {
                origin.line_number = i;
                origin.row_number = j;
            }
            if (this->map[i][j].type == Point::PointType::Terminal) {
                terminal.line_number = i;
                terminal.row_number = j;
            }
        }
    }
    // 检查起点和终点是否有效
    if (origin.line_number == -1 || origin.row_number == -1 ||
            terminal.line_number == -1 || terminal.row_number == -1) {
        /*cerr << "起点或终点未找到";*/
    }
}

bool Maze::is_invalid_position(int line, int row, const int &dline, const int &drow) {
    line = line + dline;
    row = row + drow;
    // 先检测是否越界，再检测是否为路径，否则访问路径时会下标越界
    return line < 0 || line >= line_number || row < 0 || row >= row_number
            || map[line][row].type == Point::PointType::Wall || map[line][row].type == Point::PointType::Visited
            || map[line][row].type == Point::PointType::Origin || map[line][row].type == Point::PointType::Surround;
}

bool Maze::is_terminal(const Point &point) {
    return point.line_number == terminal.line_number && point.row_number == terminal.row_number;
}

int Maze::calc_total_cost(const Point &point) {
    return point.current_cost + abs(point.line_number - terminal.line_number) + abs(point.row_number - terminal.row_number);
}

void Maze::search_shortest_path() {
    for (Point* current_point = terminal.pre; current_point != &map[origin.line_number][origin.row_number]; current_point = current_point->pre) {
        current_point->type = Point::PointType::ShortestPath;
        update();
    }
}

// 执行后检查一次当前点,将整个A*操作分解不一次执行完，便于调试
int Maze::AStarSearchOnce() {
    // 如果是第一次执行，则将起点加入队列
    if(!is_AStar_started) {
        AStar_queue.push(origin);
        is_AStar_started = true;
    }
    // 没有找到路径
    if (AStar_queue.empty()) return SearchStatus::Failed;
    // 取出代价最小的元素， 并设置为visited
    Point current_point = AStar_queue.top();
    if (map[current_point.line_number][current_point.row_number].type != Point::PointType::Origin) map[current_point.line_number][current_point.row_number].type = Point::PointType::Visiting;
    AStar_queue.pop();
    // 访问周边元素
    int dline[] = { -1, 1, 0, 0 }, drow[] = { 0,0,-1,1 };
    for (int i = 0; i < 4; i++) {
        // 判断位置是否合法
        if (is_invalid_position(current_point.line_number, current_point.row_number, dline[i], drow[i])) continue;

        Point surround_point(current_point.line_number + dline[i], current_point.row_number + drow[i]);
        // 判断位置是否为终点
        if (is_terminal(map[surround_point.line_number][surround_point.row_number])) {
            terminal.pre = &map[current_point.line_number][current_point.row_number];
            search_shortest_path();
            return SearchStatus::Success;
        }
        // 设置为surround，设置pre, 设置current_cost，设置total_cost
        map[surround_point.line_number][surround_point.row_number].type = Point::PointType::Surround;
        if (map[surround_point.line_number][surround_point.row_number].pre == nullptr) map[surround_point.line_number][surround_point.row_number].pre = &map[current_point.line_number][current_point.row_number];// 已经有pre的不再修改，保证最短路径
        map[surround_point.line_number][surround_point.row_number].current_cost = current_point.current_cost + 1;// 当前代价为父节点当前代价+1
        map[surround_point.line_number][surround_point.row_number].total_cost = calc_total_cost(surround_point);// 当前代价为父节点当前代价+1
        // 将点加入优先队列
        AStar_queue.push(map[surround_point.line_number][surround_point.row_number]);
    }
    /* 调试用 */

    // 将正在visiting点加入visiting
    if (map[current_point.line_number][current_point.row_number].type != Point::PointType::Origin) map[current_point.line_number][current_point.row_number].type = Point::PointType::Visited;
    // 返回结果
    return SearchStatus::Searching;
}

void Maze::refresh()
{
    // 清空A*队列, 重置搜索状态
    while(!AStar_queue.empty())
        AStar_queue.pop();
    is_AStar_started = false;
    // 清空地图与地图重新生成
    QRandomGenerator *generator = QRandomGenerator::global();// 随机数生成
    for (int i = 0; i < line_number; i++) {
        for (int j = 0; j < row_number; j++) {
            // 墙壁和路面设置
            if (generator->bounded(0, 10) < 8) map[i][j].type = Point::PointType::Path;
            else map[i][j].type = Point::PointType::Wall;
            // 重置元素Point参数
            map[i][j].total_cost = 0;
            map[i][j].current_cost = 0;
            map[i][j].pre = nullptr;
        }
    }
    map[0][0].type = Point::PointType::Origin;
    map[line_number - 1][row_number - 1].type = Point::PointType::Terminal;
}

// 一次执行完
bool Maze::AStar() {
    // 如果是第一次执行，则将起点加入队列
    if(!is_AStar_started) {
        AStar_queue.push(origin);
        is_AStar_started = true;
    }
    while (!AStar_queue.empty()) {
        // 取出代价最小的元素， 并设置为visited
        Point current_point = AStar_queue.top();
        if (map[current_point.line_number][current_point.row_number].type != Point::PointType::Origin) map[current_point.line_number][current_point.row_number].type = Point::PointType::Visiting;
        AStar_queue.pop();
        // 访问周边元素
        int dline[] = { -1, 1, 0, 0 }, drow[] = { 0,0,-1,1 };
        for (int i = 0; i < 4; i++) {
            // 判断位置是否合法
            if (is_invalid_position(current_point.line_number, current_point.row_number, dline[i], drow[i])) continue;

            Point surround_point(current_point.line_number + dline[i], current_point.row_number + drow[i]);
            // 判断位置是否为终点
            if (is_terminal(map[surround_point.line_number][surround_point.row_number])) {
                terminal.pre = &map[current_point.line_number][current_point.row_number];
                search_shortest_path();
                return true;
            }
            // 设置为surround，设置pre, 设置current_cost，设置total_cost
            map[surround_point.line_number][surround_point.row_number].type = Point::PointType::Surround;
            if (map[surround_point.line_number][surround_point.row_number].pre == nullptr) map[surround_point.line_number][surround_point.row_number].pre = &map[current_point.line_number][current_point.row_number];// 已经有pre的不再修改，保证最短路径
            map[surround_point.line_number][surround_point.row_number].current_cost = current_point.current_cost + 1;// 当前代价为父节点当前代价+1
            map[surround_point.line_number][surround_point.row_number].total_cost = calc_total_cost(surround_point);// 当前代价为父节点当前代价+1
            // 将点加入优先队列
            AStar_queue.push(map[surround_point.line_number][surround_point.row_number]);
        }
        /* 调试用 */

        // 将正在visiting点加入visiting
        if (map[current_point.line_number][current_point.row_number].type != Point::PointType::Origin) map[current_point.line_number][current_point.row_number].type = Point::PointType::Visited;
    }
    return false;
}

void Maze::draw(QPainter* painter)
{
    // 绘制地图元素：起点，终点...
    for (int i = 0; i < line_number; i++) {
        for (int j = 0; j < row_number; j++) {
            QBrush brush(Qt::green);
            switch (map[i][j].type)
            {
            case Point::PointType::Path:
                brush.setColor(Qt::transparent);
                break;
            case Point::PointType::Wall:
                brush.setColor(Qt::red);
                break;
            case Point::PointType::Origin:
                brush.setColor(Qt::magenta);
                break;
            case Point::PointType::Terminal:
                brush.setColor(Qt::magenta);
                break;
            case Point::PointType::Visiting:
                brush.setColor(Qt::white);
                break;
            case Point::PointType::Visited:
                brush.setColor(Qt::cyan);
                break;
            case Point::PointType::Surround:
                brush.setColor(Qt::yellow);
                break;
            case Point::PointType::ShortestPath:
                brush.setColor(Qt::green);
                break;
            }
            painter->setBrush(brush);
            painter->drawRect(j * width, i * height, width, height);
        }
    }
}
