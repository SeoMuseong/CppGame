#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsscene>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsview>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QScreen>
#include <random>
#include "Consts.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QScreen* screen = a.primaryScreen();

    QRect geometry = screen->geometry();
    QGraphicsScene scene;
    scene.setSceneRect(geometry);

    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<int> dis(0, 11);

    QPixmap pixmap(Consts::paths[dis(gen)].c_str());
    QGraphicsPixmapItem item(pixmap);

    scene.addItem(&item);
     
    QGraphicsView view(&scene);
    view.showFullScreen();
//    view.show();

    return a.exec();
}
