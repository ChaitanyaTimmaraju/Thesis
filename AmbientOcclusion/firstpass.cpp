#include "firstpass.h"
#include <QOpenGLVertexArrayObject>

void FirstPass::setUniforms() {
  m_shaderHandlerObject.m_program->bind();
  m_shaderHandlerObject.m_program->setUniformValue(m_transformID,
                                                   m_transform.toMatrix());
  m_shaderHandlerObject.m_program->setUniformValue(m_projectionID,
                                                   m_projection);
}

void FirstPass::initializations(std::vector<ModelLoader*>& models) {
  m_shaderHandlerObject.compileShaders(":/shaders/FirstPass.vert",
                                       ":/shaders/firstpass.geom",
                                       ":/shaders/FirstPass.frag");

  m_shaderHandlerObject.m_program->bind();
  m_transformID =
      m_shaderHandlerObject.m_program->uniformLocation("modelToWorld");
  m_projectionID =
      m_shaderHandlerObject.m_program->uniformLocation("worldToView");

  for (auto x : models) {
    QOpenGLVertexArrayObject* tempVAO = new QOpenGLVertexArrayObject();
    tempVAO->create();
    tempVAO->bind();

    // Bind the required buffers and set the required information.
    x->m_vertexID.bind();

    m_shaderHandlerObject.m_program->enableAttributeArray(0);
    m_shaderHandlerObject.m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);

    x->m_normalID.bind();
    m_shaderHandlerObject.m_program->enableAttributeArray(1);
    m_shaderHandlerObject.m_program->setAttributeBuffer(1, GL_FLOAT, 0, 3, 0);
    /*
     * Additional data if you need tangents and texture coordintaes
       x->m_tangentsID.bind();
       m_shaderHandlerObject.m_program->enableAttributeArray(2);
       m_shaderHandlerObject.m_program->setAttributeBuffer(2, GL_FLOAT, 0, 3,
     0);

       x->m_uvID.bind();
       m_shaderHandlerObject.m_program->enableAttributeArray(3);
       m_shaderHandlerObject.m_program->setAttributeBuffer(3, GL_FLOAT, 0, 2,
     0);
   */

    // Don't forget to bind indices otherwise model will not draw.
    x->m_indicesID->bind();

    // push back to our local stack
    m_VAO.push_back(tempVAO);
    // releasing all information about the model.
    tempVAO->release();
    x->m_indicesID->release();
    x->m_vertexID.release();
    x->m_normalID.release();
  }
  // release the program
  m_shaderHandlerObject.m_program->release();
}

void FirstPass::setObjectData(int indexOfObjectCurrentlyDrawn) {
  m_VAO[indexOfObjectCurrentlyDrawn]->bind();
}

void FirstPass::releaseProgramAndObjectData() {
  for (auto temp : m_VAO) {
    temp->release();
  }
  m_shaderHandlerObject.m_program->release();
}
