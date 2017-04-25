#include "modelloader.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
ModelLoader::ModelLoader()
{
    m_indicesID = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
}

ModelLoader::ModelLoader(QString fileName): ModelLoader()
{
    loadObjectAndSetBuffers(fileName);
}

void ModelLoader::loadObjectAndSetBuffers(QString fileName)
{
        QFile file(fileName);

        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
         {
               QTextStream fileText(&file);

               while(!fileText.atEnd())
               {
                QString fileLine = fileText.readLine();
                QVector3D temp;
                QVector2D tempUV;

                if(fileLine.startsWith("v "))
                {
                    QStringList list = fileLine.split(" ");
                    temp.setX(list[1].toFloat());
                    temp.setY(list[2].toFloat());
                    temp.setZ(list[3].toFloat());
                    vertices.push_back(temp);
                }else if(fileLine.startsWith("vn ")){
                    QStringList list = fileLine.split(" ");
                    temp.setX(list[1].toFloat());
                    temp.setY(list[2].toFloat());
                    temp.setZ(list[3].toFloat());
                    normals.push_back(temp);
                }else if(fileLine.startsWith("vtangent ")){
                    QStringList list = fileLine.split(" ");
                    temp.setX(list[1].toFloat());
                    temp.setY(list[2].toFloat());
                    temp.setZ(list[3].toFloat());
                    tangents.push_back(temp);
                }else if(fileLine.startsWith("vt ")){
                    QStringList list = fileLine.split(" ");
                    tempUV.setX(list[1].toFloat());
                    tempUV.setY(list[2].toFloat());
                    uvs.push_back(tempUV);
                }else if(fileLine.startsWith("f ")){
                    QStringList list = fileLine.split(" ");
                    indices.push_back(list[1].toInt()-1);
                    indices.push_back(list[2].toInt()-1);
                    indices.push_back(list[3].toInt()-1);
                }else if(fileLine.startsWith("ts "))
                        break;

               }
          }else
            qDebug()<<qPrintable(fileName)<<" not found.(Make sure to add in resources.qrc)";


        //create Index Buffer
        m_indicesID->create();
        m_indicesID->bind();
        m_indicesID->setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_indicesID->allocate(indices.constData(),indices.size()*sizeof(unsigned int));
        m_indicesID->release();

        //Create Vertex and other Buffers.
        m_vertexID.create();
        m_vertexID.bind();
        m_vertexID.setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_vertexID.allocate(vertices.constData(),vertices.size()*sizeof(QVector3D));
        m_vertexID.release();

        m_normalID.create();
        m_normalID.bind();
        m_normalID.setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_normalID.allocate(normals.constData(),normals.size()*sizeof(QVector3D));
        m_normalID.release();

        m_tangentsID.create();
        m_tangentsID.bind();
        m_tangentsID.setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_tangentsID.allocate(tangents.constData(),tangents.size()*sizeof(QVector3D));
        m_tangentsID.release();

        m_uvID.create();
        m_uvID.bind();
        m_uvID.setUsagePattern(QOpenGLBuffer::StaticDraw);
        m_uvID.allocate(uvs.constData(),uvs.size()*sizeof(QVector2D));
        m_uvID.release();

}

void ModelLoader::draw()
{
   // glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);

}

ModelLoader::~ModelLoader()
{
    m_vertexID.destroy();
    m_indicesID->destroy();
    m_uvID.destroy();
    m_tangentsID.destroy();
    m_normalID.destroy();
}
