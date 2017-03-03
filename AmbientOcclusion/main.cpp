#include <QGuiApplication>
#include "window.h"
int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    //set OpenGL Version information.
    //Note: This format must be before show() is called.
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(4,3);

    //set the window up
    Window window;
    window.setFormat(format);
    window.resize(QSize(800,600));
    window.show();

    return a.exec();
}
