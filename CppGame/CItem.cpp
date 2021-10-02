#include "CItem.h"
#include "Consts.h"
CItem::CItem(const std::string& path, int row, int column, QGraphicsItem* parent)
	:QGraphicsPixmapItem(QPixmap(path.c_str()).scaled(Consts::BOARD_ITEM_SIZE, Consts::BOARD_ITEM_SIZE), parent)
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
