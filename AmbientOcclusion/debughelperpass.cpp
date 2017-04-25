#include "debughelperpass.h"
#include <QOpenGLVertexArrayObject>

void DebugHelperPass::setUniforms()
{
    m_shaderHandlerObject.m_program->bind();
    m_shaderHandlerObject.m_program->setUniformValue(m_transformID,m_transform.toMatrix());
    m_shaderHandlerObject.m_program->setUniformValue(m_projectionID,m_projection);
}

void DebugHelperPass::initializations(std::vector<ModelLoader*>& models)
{

    m_shaderHandlerObject.compileShaders(":/shaders/normalshader.vert",":/shaders/normalshader.geom",":/shaders/normalshader.frag");

    m_shaderHandlerObject.m_program->bind();

    //m_transform.translate(0.0,0.0,-2.0);

    m_transformID = m_shaderHandlerObject.m_program->uniformLocation("modelToWorld");
    m_projectionID = m_shaderHandlerObject.m_program->uniformLocation("worldToView");

    for(auto x: models)
    {
        QOpenGLVertexArrayObject *tempVAO = new QOpenGLVertexArrayObject();
        tempVAO->create();
        tempVAO->bind();

        //Bind the required buffers and set the required information.
        x->m_vertexID.bind();

        m_shaderHandlerObject.m_program->enableAttributeArray(0);
        m_shaderHandlerObject.m_program->setAttributeBuffer(0,GL_FLOAT,0,3,0);

        x->m_normalID.bind();
        m_shaderHandlerObject.m_program->enableAttributeArray(1);
        m_shaderHandlerObject.m_program->setAttributeBuffer(1,GL_FLOAT,0,3,0);

        x->m_tangentsID.bind();
        m_shaderHandlerObject.m_program->enableAttributeArray(2);
        m_shaderHandlerObject.m_program->setAttributeBuffer(2,GL_FLOAT,0,3,0);

        //Don't forget to bind indices otherwise model will not draw.
        x->m_indicesID->bind();

        //push back to our local queue
        m_VAO.push_back(tempVAO);
        //releasing all information about the model.
        tempVAO->release();
        x->m_indicesID->release();
        x->m_vertexID.release();
        x->m_normalID.release();
    }
    //release the program
    m_shaderHandlerObject.m_program->release();
}

void DebugHelperPass::setObjectData(int indexOfObjectCurrentlyDrawn)
{
    m_VAO[indexOfObjectCurrentlyDrawn]->bind();

}

void DebugHelperPass::releaseProgramAndObjectData()
{
    for(auto temp:m_VAO)
    {
        temp->release();
    }
    m_shaderHandlerObject.m_program->release();

}
