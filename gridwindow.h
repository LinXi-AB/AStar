#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H

#include <QMainWindow>
#include "grid.h"
#include "maze.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GridWindow; }
QT_END_NAMESPACE

class GridWindow : public QMainWindow
{
    Q_OBJECT

public:
    GridWindow(QWidget *parent = nullptr);
    ~GridWindow();
    void paintEvent(QPaintEvent *event);
    void drawGrid();
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::GridWindow *ui;
    Grid* grid;
    Maze* maze;
    int width = 30;
    int height = 30;
    int line_number = 25;
    int row_number = 25;
    qreal scaleX = 1;
    qreal scaleY = 1;
    bool isSearchOver = false;// 搜索是否结束，结束后不允许继续搜索

signals:
    void continue_search();
};
#endif // GRIDWINDOW_H
