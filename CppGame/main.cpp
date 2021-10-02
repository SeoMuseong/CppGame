#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsscene>
#include <QtWidgets/QGraphicsview>
#include <QScreen>
#include "Consts.h"
#include "CBoard.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QScreen* screen = a.primaryScreen();

    QRect geometry = screen->geometry();
    QGraphicsScene scene;
    scene.setSceneRect(geometry);

    CBoard board(&scene);

     
    QGraphicsView view(&scene);
    view.showFullScreen();
//    view.show();

    return a.exec();
}
