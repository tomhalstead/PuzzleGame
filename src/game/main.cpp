#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include "glworld.h"
#include "glmainmenu.h"

int main(int argc, char* argv[])
{
    QApplication a(argc,argv);
    qDebug() << "Create GLWindow";
    GLWindow window;
    window.resize(QApplication::desktop()->size());
    qDebug() << "Create GLMainMenu";
    GLMainMenu *menu = new GLMainMenu(&window);
    qDebug() << "Push main menu";
    window.Push(new GLWindowInfo(menu),true);
    qDebug() << "Show full screen";
    window.showFullScreen();
    window.Start();
    return a.exec();
}
