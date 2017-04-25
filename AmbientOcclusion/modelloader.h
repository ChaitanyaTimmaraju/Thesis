#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <QString>
#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include <QOpenGLBuffer>

class ModelLoader
{

public:
    ModelLoader();
    ~ModelLoader();
    ModelLoader(QString fileName);
    void loadObjectAndSetBuffers(QString fileName);
    void draw();
    //OpenGL state information
    QOpenGLBuffer m_vertexID,m_normalID,m_tangentsID,m_uvID;
    QOpenGLBuffer *m_indicesID;

    //object Data
    QVector<QVector3D> vertices,normals,tangents;
    QVector<unsigned int> indices;
    QVector<QVector2D> uvs;



};

#endif // MODELLOADER_H
