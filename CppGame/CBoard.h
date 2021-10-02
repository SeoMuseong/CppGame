#pragma once
#include <QtWidgets/QGraphicsscene>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsRectItem>
#include <vector>
#include <random>

class CBoard
{
private:
	QGraphicsScene* _scene;
	QGraphicsRectItem _root;
	std::vector<std::vector< QGraphicsPixmapItem*>> _items;
	std::random_device _device;
	std::mt19937 _gen;
public:
	CBoard(QGraphicsScene* scene);
	~CBoard();
	void addItem(int row, int column);
	void removeItem(int row, int column);
};