#include "CItem.h"
#include "Consts.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>

CItem::CItem(EventLister* listner, const std::string& path, int row, int column, QGraphicsItem* parent)
	:QGraphicsPixmapItem(QPixmap(path.c_str()).scaled(Consts::BOARD_ITEM_SIZE, Consts::BOARD_ITEM_SIZE), parent)
	, _listner(listner)
	, _path(path)
	, _row(row)
	, _column(column)
{
	/*
		QPixmap pixmap(path].c_str());
	QPixmap scaled(pixmap.scaled(Consts::BOARD_ITEM_SIZE, Consts::BOARD_ITEM_SIZE));
	QGraphicsPixmapItem *item = new QGraphicsPixmapItem(scaled, &_root); 
	*/
}

std::string CItem::getPath() const
{
	return _path;
}

int CItem::getRow() const
{
	return _row;
}

int CItem::getColumn() const
{
	return _column;
}

void CItem::SetRow(int row)
{
	_row = row;
}

void CItem::SetColumn(int column)
{
	_column = column;
}

void CItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	this->_pressPos = event->pos();
}

void CItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QPointF releasePos = event->pos();
	QPointF diff = releasePos - _pressPos;
	Direction dir;
	if (diff.x() == 0 && diff.y() == 0)
		return;
	if (qAbs(diff.x()) > qAbs(diff.y()))
	{
		if (diff.x() > 0)
		{
			dir = Direction::Right;
		}
		else
		{
			dir = Direction::Left;
		}
	}
	else
	{
		if (diff.y() > 0)
		{
			dir = Direction::Down;
		}
		else
		{
			dir = Direction::Up;
		}
	}
	_listner->itemDragEvent(this, dir);
}
