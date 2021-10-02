#include "CBoard.h"
#include "Consts.h"
#include "CItem.h"
#include <random>
#include <qdebug.h>
CBoard::CBoard(QGraphicsScene* scene)
	:_scene(scene),
	_gen(_device())
{
	scene->addItem(&_root);
	_root.setX(scene->sceneRect().width() / 2 - Consts::BOARD_ITEM_SIZE * (Consts::BOARD_LENGTH / 2) );
	_root.setY(scene->sceneRect().height() / 2 - Consts::BOARD_ITEM_SIZE * (Consts::BOARD_LENGTH / 2) );
	for (int row = 0; row < Consts::BOARD_LENGTH; row++)
	{
		std::vector< CItem*> rowItem(Consts::BOARD_LENGTH);
		_items.push_back(rowItem);
		for (int column = 0; column < Consts::BOARD_LENGTH; column++)
		{
			addItem(row, column);
		}
	}

}

CBoard::~CBoard()
{
	for (int row = 0; row < _items.size(); row++)
	{
		for (int column = 0; column < _items[row].size();  column++)
		{
			removeItem(row, column);
		}
	}
}

void CBoard::addItem(int row, int column)
{
	std::uniform_int_distribution<int> dis(0, 11);

	auto& path = Consts::paths[dis(_gen)];

	CItem* item = new CItem(this, path, row, column, &_root);

	item->setPos(column * Consts::BOARD_ITEM_SIZE, row * Consts::BOARD_ITEM_SIZE);

	_items[row][column] = item;
}

void CBoard::removeItem(int row, int column)
{
	auto* item = _items[row][column];

	if (item == nullptr)
		return;

	_items[row][column] = nullptr;
	item->setParentItem(nullptr);
	_scene->removeItem(item);

	delete item;
}

void CBoard::moveItem(CItem* item, int toRow, int toColumn)
{
	item->SetRow(toRow);
	item->SetColumn(toColumn);

	item->setPos(toColumn * Consts::BOARD_ITEM_SIZE, toRow * Consts::BOARD_ITEM_SIZE);

	_items[toRow][toColumn] = item;
}

void CBoard::exchange(int row0, int column0, int row1, int column1)
{
	CItem* item0 = _items[row0][column0];
	CItem* item1 = _items[row1][column1];

	moveItem(item0, row1, column1);
	moveItem(item1, row0, column0);
}

void CBoard::itemDragEvent(CItem* item, CItem::Direction dir)
{
	int row0 = item->getRow();
	int column0 = item->getColumn();

	int row1 = row0;
	int column1 = column0;

	switch (dir)
	{
	case CItem::Direction::Up:
		row1--;
		break;
	case CItem::Direction::Down:
		row1++;
		break;
	case CItem::Direction::Left:
		column1--;
		break;
	case CItem::Direction::Right:
		column1++;
		break;
	}

	if (column1 < 0 || row1 < 0)
		return;
	if (column1 >= Consts::BOARD_LENGTH || row1 >= Consts::BOARD_LENGTH)
		return;

	CItem* item1 = _items[row1][column1];
	if (item1 == nullptr)
		return;

	exchange(row0, column0, row1, column1);

}
