#include <QGuiApplication>
#include "window.h"
#include <QtQuick/QQuickView>
#include <QtQml>

int main(int argc, char* argv[]) {
  QGuiApplication a(argc, argv);
  qmlRegisterType<Window>("com.window.launcher", 1, 0, "Launcher");
  // set OpenGL Version information.
  // Note: This format must be before show() is called.
  QSurfaceFormat format;
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 4);

  // set the window up
  Window window;
  window.setFormat(format);
  window.resize(QSize(1024, 1024));
  window.setFramePosition(QPoint(500, 25));
  window.show();

  QQuickView view;
  view.setSource(QUrl("mainUI.qml"));
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.setFramePosition(QPoint(200, 25));

  view.show();

  return a.exec();
}
