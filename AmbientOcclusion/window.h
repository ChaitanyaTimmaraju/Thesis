#ifndef WINDOW_H
#define WINDOW_H
/*Inherits Qt5Gui-based classes.Allows us to provide OpenGL abstractions without need
for Qt5 widgets module*/
#include <QOpenGLWindow>
/*Access to QOpenGL ES 2.0 API*/
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_1>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QMap>
#include <QString>
#include <QOpenGLVertexArrayObject>
#include <vector>
#include "transform3d.h"

class ModelLoader;
class Window : public QOpenGLWindow,
               protected QOpenGLFunctions_3_1
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
    void keyPressEvent(QKeyEvent *e);
    void delay(float f);
    void createFrameBuffer();

    QMap<QString,QString> objectFilePairs;
    Transform3D m_transform;
    QMatrix4x4 m_projection;
    std::vector<ModelLoader*> models;
    std::vector<ModelLoader*> secondPassModels;

    //FrameBuffer informations
    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION0,
        GBUFFER_TEXTURE_TYPE_POSITION1,
        GBUFFER_TEXTURE_TYPE_POSITION2,
        GBUFFER_NORMAL_TEXTURE,
        GBUFFER_NUM_TEXTURES
    };
    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);
    void BindForWriting();
    void BindForReading();
    void SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType);
    void ReadFromFrameBuffer();

protected slots:
    void update();

private:
    //private Helpers
    void printContextInformation();
    GLuint m_fbo;
    GLuint m_textures[GBUFFER_NUM_TEXTURES];
    GLuint m_depthTexture;
};

#endif // WINDOW_H
