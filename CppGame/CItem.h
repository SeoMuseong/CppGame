#pragma once
#include <QtWidgets/QGraphicsPixmapItem>
class CItem :
    public QGraphicsPixmapItem
{
private:
    const std::string _path;
    int _row;
    int _column;
public:
    CItem(const std::string& path, int row, int column, QGraphicsItem* parent);
    std::string getPath() const;
    int getRow() const;
    int getColumn() const;

    void SetRow(int row);
    void SetColumn(int column);
};

