#ifndef WINDOW_H
#define WINDOW_H
/*Inherits Qt5Gui-based classes.Allows us to provide OpenGL abstractions without need
for Qt5 widgets module*/
#include<QOpenGLWindow>

/*Access to QOpenGL ES 2.0 API*/
#include<QOpenGLFunctions>
#include<QOpenGLBuffer>
#include<QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include "transform3d.h"

class QOpenGLShaderProgram;

class Window : public QOpenGLWindow,
               protected QOpenGLFunctions
{
    //QT's object
    Q_OBJECT

//OpenGL Events
public:
    Window();
    ~Window();

    void initializeGL();
    void resizeGL(int width,int height);
    void paintGL();
    void teardownGL();

protected slots:
    void update();

private:
    //OpenGL state information
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;

    //private Helpers
    void printContextInformation();

    //shader Information
    int u_modelToWorld;
    int u_worldToView;
    QMatrix4x4 m_projection;
    Transform3D m_transform;
};

#endif // WINDOW_H
