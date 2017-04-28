#ifndef WINDOW_H
#define WINDOW_H
/*Inherits Qt5Gui-based classes.Allows us to provide OpenGL abstractions without
need
for Qt5 widgets module*/
#include <QOpenGLWindow>
/*Access to QOpenGL ES 2.0 API*/
#include <QOpenGLFunctions_4_4_Core>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QMap>
#include <QString>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>
#include <vector>
#include "transform3d.h"

class ModelLoader;
class Window : public QOpenGLWindow, protected QOpenGLFunctions_4_4_Core {
  // QT's object
  Q_OBJECT

  // OpenGL Events
 public:
  Window();
  ~Window();
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
  void teardownGL();
  void keyPressEvent(QKeyEvent* e);
  void delay(float f);
  void createFrameBuffer();

  QMap<QString, QString> objectFilePairs;
  Transform3D m_transform;
  QMatrix4x4 m_projection;
  std::vector<ModelLoader*> models;
  std::vector<ModelLoader*> secondPassModels;

  void saveTexturesToFiles();
  void setFrameBuffer();
  void setTextures();
  GLenum glCheckError_(QString file, int line);

 protected slots:
  void update();

 private:
  // private Helpers
  void printContextInformation();
  QOpenGLFramebufferObject* fbo;
};

#endif  // WINDOW_H
