#include "secondpass.h"

#include <QOpenGLVertexArrayObject>
#include <cstdlib>
#include <ctime>
#include <QVector3D>

void SecondPass::setUniforms() {
  m_shaderHandlerObject.m_program->bind();
  m_shaderHandlerObject.m_program->setUniformValue(m_transformID,
                                                   m_transform.toMatrix());
  m_shaderHandlerObject.m_program->setUniformValue(m_projectionID,
                                                   m_projection);
}

void SecondPass::initializations(std::vector<ModelLoader*>& models) {
  // Initialize srand with same seed value every time
  std::srand(static_cast<unsigned int>(time(0)));

  m_shaderHandlerObject.compileShaders(":/shaders/secondpass.vert", NULL,
                                       ":/shaders/secondpass.frag");

  m_shaderHandlerObject.m_program->bind();

  m_transformID =
      m_shaderHandlerObject.m_program->uniformLocation("modelToWorld");
  m_projectionID =
      m_shaderHandlerObject.m_program->uniformLocation("worldToView");

  m_shaderHandlerObject.m_program->setUniformValue("firstPassSamplers[0]", 0);
  m_shaderHandlerObject.m_program->setUniformValue("firstPassSamplers[1]", 1);
  m_shaderHandlerObject.m_program->setUniformValue("firstPassSamplers[2]", 2);
  m_shaderHandlerObject.m_program->setUniformValue("firstPassSamplers[3]", 3);
  m_shaderHandlerObject.m_program->setUniformValue("displayOption", 8);
  m_shaderHandlerObject.m_program->setUniformValue("EPSILON", -0.0f);
  m_shaderHandlerObject.m_program->setUniformValue("mouseClickRay",
                                                   QVector3D(0.0, 0.0, 1.0));

  m_shaderHandlerObject.m_program->setUniformValue("screenWidth", 1024.0f);
  m_shaderHandlerObject.m_program->setUniformValue("screenHeight", 1024.0f);

  // Generate Sample kernel vectors and set uniforms
  for (int i = 0; i < sampleSize; ++i) {
    float scale = i / sampleSize;
    QVector3D temp;
    QString uniformName("fRandom_Vectors[");
    uniformName += QString::number(i) + "]";

    temp[0] = 2.0 * rand() - 1.0;
    temp[1] = 2.0 * rand() - 1.0;
    temp[2] = 2.0 * rand() - 1.0;
    // Use an acceleration function so more points are
    // located closer to the origin
    temp[0] *= (0.1 + (0.9 * scale * scale));
    temp[1] *= (0.1 + (0.9 * scale * scale));
    temp[2] *= (0.1 + (0.9 * scale * scale));
    temp.normalize();
    // qDebug() << qPrintable("fRandomVectors[" + QString::number(i) +
    // "]=vec3(")
    //          << temp[0] << "," << temp[1] << "," << temp[2] << ");";
    m_shaderHandlerObject.m_program->setUniformValue(
        uniformName.toStdString().c_str(), temp);
  }

  for (auto x : models) {
    QOpenGLVertexArrayObject* tempVAO = new QOpenGLVertexArrayObject();
    tempVAO->create();
    tempVAO->bind();

    // Bind the required buffers and set the required information.
    x->m_vertexID.bind();

    m_shaderHandlerObject.m_program->enableAttributeArray(0);
    m_shaderHandlerObject.m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 0);
    /*
        x->m_normalID.bind();
        m_shaderHandlerObject.m_program->enableAttributeArray(1);
        m_shaderHandlerObject.m_program->setAttributeBuffer(1, GL_FLOAT, 0, 3,
       0);

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

    // push back to our local queue
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

void SecondPass::setObjectData(int indexOfObjectCurrentlyDrawn) {
  m_VAO[indexOfObjectCurrentlyDrawn]->bind();
}

void SecondPass::releaseProgramAndObjectData() {
  for (auto temp : m_VAO) {
    temp->release();
  }
  m_shaderHandlerObject.m_program->release();
}
