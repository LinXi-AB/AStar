#include "grid.h"
#include <QPainter>

Grid::Grid(QWidget *parent)
    : QWidget{parent}
{

}

Grid::Grid(int width, int height, int line_number, int row_number, QWidget *parent)
    : QWidget{parent}
{
    this->width = width;
    this->height = height;
    this->line_number = line_number;
    this->row_number = row_number;
}

void Grid::draw(QPainter *painter)
{
    // 显示逻辑
    for (int i = 0; i < row_number; i++) {
        for (int j = 0; j < line_number; j++) {
            painter->setRenderHint(QPainter::Antialiasing);
            painter->drawRect(i * width, j * height, width, height);
        }
    }
}
