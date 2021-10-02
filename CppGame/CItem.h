#pragma once
#include <QtWidgets/QGraphicsPixmapItem>
class CItem :
    public QGraphicsPixmapItem
{
    class EventLister;
private:
    const std::string _path;
    int _row;
    int _column;
    EventLister* _listner;
    QPointF _pressPos;
public:
    enum class Direction
    {
        Left, Right, Up, Down
    };
     
    class EventLister
    {
    public:
        virtual void itemDragEvent(CItem* item, CItem::Direction dir) = 0;

    };
public:
    CItem(EventLister* listner, const std::string& path, int row, int column, QGraphicsItem* parent);
    std::string getPath() const;
    int getRow() const;
    int getColumn() const;

    void SetRow(int row);
    void SetColumn(int column);
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};
