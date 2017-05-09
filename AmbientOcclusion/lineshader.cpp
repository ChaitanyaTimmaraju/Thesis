#include "lineshader.h"
#include <QOpenGLVertexArrayObject>
void LineShader::setUniforms() {
  m_shaderHandlerObject.m_program->bind();
  m_shaderHandlerObject.m_program->setUniformValue(m_projectionID,
                                                   m_projection);
  tempVAO->bind();
}
void LineShader::setVertices() {
  // Bind the required buffers and set the required information.
  m_vertexID.create();
  m_vertexID.bind();
  m_vertexID.setUsagePattern(QOpenGLBuffer::StaticDraw);
  m_vertexID.allocate(vertices.constData(),
                      vertices.size() * sizeof(QVector3D));
  m_vertexID.release();
}

void LineShader::initializations(std::vector<ModelLoader*>& models) {
  m_shaderHandlerObject.compileShaders(":/shaders/lineshader.vert", NULL,
                                       ":/shaders/lineshader.frag");

  vertices.push_back(QVector3D(0.0, 0.0, -0.1));
  vertices.push_back(QVector3D(0.0, 0.5, -3.0));
  setVertices();
  m_shaderHandlerObject.m_program->bind();

  m_projectionID =
      m_shaderHandlerObject.m_program->uniformLocation("worldToView");
  tempVAO = new QOpenGLVertexArrayObject();
  tempVAO->create();
  tempVAO->bind();
  m_vertexID.bind();
  m_shaderHandlerObject.m_program->enableAttributeArray(0);
  m_shaderHandlerObject.m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
  tempVAO->release();
  m_shaderHandlerObject.m_program->release();

  // releasing all information about the model.
  m_vertexID.release();
}
void LineShader::setObjectData(int) {}
void LineShader::releaseProgramAndObjectData() {
  tempVAO->release();
  m_shaderHandlerObject.m_program->release();
}
