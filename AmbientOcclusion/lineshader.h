#ifndef LINESHADER_H
#define LINESHADER_H

#include "passinterface.h"
#include <QOpenGLBuffer>
#include <QVector>
#include <QVector3D>
#include <QString>
class LineShader : public PassInterface {
 public:
  LineShader() {}
  virtual void setUniforms();
  virtual void initializations(std::vector<ModelLoader*>& models);
  virtual void setObjectData(int);
  virtual void releaseProgramAndObjectData();
  void setVertices();
  QMatrix4x4 m_projection;
  int m_projectionID;
  QOpenGLBuffer m_vertexID;
  QVector<QVector3D> vertices;
  QOpenGLVertexArrayObject* tempVAO;
};
#endif  // LINESHADER_H
