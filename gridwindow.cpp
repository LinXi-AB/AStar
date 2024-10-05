#include "gridwindow.h"
#include "ui_gridwindow.h"
#include <QDebug>
#include "grid.h"
#include "maze.h"
#include <vector>
#include "point.h"
#include "QVBoxLayout"
#include <QBrush>
#include <QRandomGenerator>
#include <QKeyEvent>

GridWindow::GridWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GridWindow)
{
    ui->setupUi(this);
    // 网格创建
    grid = new Grid(width, height, line_number, row_number, this);
    // 地图创建
    QRandomGenerator *generator = QRandomGenerator::global();// 随机数生成
    std::vector<std::vector<Point>> map(line_number, std::vector<Point>(row_number));

    for (int i = 0; i < line_number; i++) {
        for (int j = 0; j < row_number; j++) {
            if (generator->bounded(0, 10) < 8) map[i][j].type = Point::PointType::Path;// 设置概率
            else map[i][j].type = Point::PointType::Wall;
        }
    }
    map[0][0].type = Point::PointType::Origin;
    map[line_number - 1][row_number - 1].type = Point::PointType::Terminal;
    maze = new Maze(map, width, height, this);// 地图的行数和列数根据传入的数组自动获取

    this->resize(width * row_number, height * line_number);
    setWindowTitle("searching...");
}

GridWindow::~GridWindow()
{
    delete ui;
    delete grid;
    delete maze;
}

void GridWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 设置painter偏移量，使图形居中显示
    int dx = (geometry().width() - width * row_number * scaleX) / 2;
    int dy = (geometry().height() - height * line_number * scaleY) / 2;
    painter.translate(dx, dy);
    // 设置缩放
    painter.scale(scaleX, scaleY);

    grid->draw(&painter);
    maze->draw(&painter);
}

void GridWindow::wheelEvent(QWheelEvent *event)
{
    // 获取滚轮的增量
    int delta = event->angleDelta().y(); // y轴的滚动量
    if (delta > 0) {
        scaleX+=0.02;
        scaleY+=0.02;
    }
    else {
        if (width * scaleX >= 20 && height * scaleY >= 20) {
            scaleX -= 0.02;
            scaleY -= 0.02;
        }
    }
    event->accept(); // 接受事件
    update();
}

void GridWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Return:// 搜索一次
        if (!isSearchOver) {
            switch (maze->AStarSearchOnce())
            {
            case Maze::SearchStatus::Searching:
                // no action
                break;
            case Maze::SearchStatus::Failed:
                isSearchOver = true;
                setWindowTitle("没有找到路径");
                break;
            case Maze::SearchStatus::Success:
                isSearchOver = true;
                setWindowTitle("成功找到路径");
                break;
            }
            update();
        }
        break;
    case Qt::Key_Space:// 持续搜索
        if (!isSearchOver) {
            if(maze->AStar())
                setWindowTitle("成功找到路径");
            else
                setWindowTitle("没有找到路径");
            isSearchOver = true;
            update();
        }
        break;
    case Qt::Key_R:// 刷新地图
        maze->refresh();
        isSearchOver = false;
        setWindowTitle("searching...");
        update();
        break;
    }
}


