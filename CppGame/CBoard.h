#pragma once
#include <QtWidgets/QGraphicsscene>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsRectItem>
#include <vector>
#include <random>

#include "CItem.h"


class CBoard : public CItem::EventLister
{
private:
	QGraphicsScene* _scene;
	QGraphicsRectItem _root;
	std::vector<std::vector< CItem*>> _items;
	std::random_device _device;
	std::mt19937 _gen;
public:
	CBoard(QGraphicsScene* scene);
	~CBoard();
	void addItem(int row, int column);
	void removeItem(int row, int column);
	void moveItem(CItem* item, int toRow, int toColumn);
	void exchange(int row0, int column0, int row1, int column1);

	virtual void itemDragEvent(CItem* item, CItem::Direction dir);
};