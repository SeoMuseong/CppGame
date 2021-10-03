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

	while(refresh());
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

void CBoard::moveItem(int fromRow, int fromColumn, int toRow, int toColumn)
{
	CItem* item = _items[fromRow][fromColumn];
	if(item)
		moveItem(item, toRow, toColumn);
}

void CBoard::exchangeItems(int row0, int column0, int row1, int column1)
{
	CItem* item0 = _items[row0][column0];
	CItem* item1 = _items[row1][column1];

	moveItem(item0, row1, column1);
	moveItem(item1, row0, column0);
}

bool CBoard::refresh()
{
	MatchSet matched = matchedItems();
	if (matched.size() < 3)
		return false;

	for (const auto& pair : matched)
	{
		removeItem(pair.first, pair.second);
	}

	// fill with existing items
	for (int column = 0; column < _items[0].size(); column++)
	{
		for (int row = _items.size() -1 ; row >= 0; row--)
		{
			if (_items[row][column] != nullptr)
				continue;
			// _items[row][column] = nullptr;
			for (int i = row - 1; i >= 0; i--)
			{
				if (_items[i][column] != nullptr)
				{
					moveItem(i, column, row, column);
					_items[i][column] = nullptr;
					break;
				}
			}
		}
	}

	// fill with new items
	for (int column = 0; column < _items[0].size(); column++)
	{
		for (int row = 0; row < _items.size(); row++)
		{
			if (_items[row][column] == nullptr)
			{
				addItem(row, column);
			}
			else
				break;
		}
	}
	return true;
}

MatchSet CBoard::matchedItems() const
{
	MatchSet matched;

	for (int row = 0; row < _items.size(); row++)
	{
		for (int column = 0; column < _items[row].size(); column++)
		{
			MatchSet m = matchedItems(row, column);
			if (m.size() >= 3)
			{
				matched.insert(m.begin(), m.end());
			}
		}
	}
	return matched;
}

MatchSet CBoard::matchedItems(int row, int column) const
{
	MatchSet horizontalMatched = matchedItemsHorizontal(row, column);
	MatchSet verticalMatched = matchedItemsVertical(row, column);

	MatchSet matched;

	if (horizontalMatched.size() >= 3)
		matched.insert(horizontalMatched.begin(), horizontalMatched.end());
	if (verticalMatched.size() >= 3)
		matched.insert(verticalMatched.begin(), verticalMatched.end());
	
	return matched;
}

MatchSet CBoard::matchedItemsHorizontal(int row, int column) const
{
	CItem* item = _items[row][column];
	if (item == nullptr)
		return {};

	MatchSet leftMatched;
	for (int i = column - 1; i >= 0; i--)
	{
		if (_items[row][i] && _items[row][i]->getPath() == item->getPath())
			leftMatched.insert({ row, i });
		else
			break;
	}
	MatchSet rightMatched;
	for (int i = column + 1; i < _items[row].size(); i++)
	{
		if (_items[row][i] && _items[row][i]->getPath() == item->getPath())
			rightMatched.insert({ row, i });
		else
			break;
	}

	if (leftMatched.size() + rightMatched.size() + 1 >= 3)
	{
		leftMatched.insert({ row, column });
		leftMatched.insert(rightMatched.begin(), rightMatched.end());
		return leftMatched;
	}
	else
		return {};
}

MatchSet CBoard::matchedItemsVertical(int row, int column) const
{
	CItem* item = _items[row][column];
	if (item == nullptr)
		return {};

	MatchSet upMatched;
	for (int i = row - 1; i >= 0; i--)
	{
		if (_items[i][column] && _items[i][column]->getPath() == item->getPath())
			upMatched.insert({ i, column });
		else
			break;
	}

	MatchSet downMatched;
	for (int i = row +1 ; i < _items.size(); i++)
	{
		if (_items[i][column] && _items[i][column]->getPath() == item->getPath())
			downMatched.insert({ i, column });
		else
			break;
	}
	if (upMatched.size() + downMatched.size() + 1 >= 3)
	{
		upMatched.insert({ row, column });
		upMatched.insert(downMatched.begin(), downMatched.end());
		return upMatched;
	}
	else
		return {};
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

	exchangeItems(row0, column0, row1, column1);
	while (refresh());
}
