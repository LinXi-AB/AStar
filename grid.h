#ifndef GRID_H
#define GRID_H

#include <QWidget>

class Grid : public QWidget
{
    Q_OBJECT
public:
    explicit Grid(QWidget *parent = nullptr);
    explicit Grid(int width, int height, int line_number, int row_number, QWidget *parent = nullptr);
    void draw(QPainter* painter);

private:
    int width = 50;
    int height = 50;
    int line_number = 10;
    int row_number = 10;

signals:

};

#endif // GRID_H
