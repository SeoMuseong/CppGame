#include "CBoard.h"
#include "Consts.h"
#include "CItem.h"
#include <random>
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

	CItem* item = new CItem(path, row, column, &_root);

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
