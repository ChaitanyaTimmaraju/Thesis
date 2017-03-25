#include "firstpass.h"
#include <QOpenGLVertexArrayObject>


void FirstPass::setUniforms()
{
    m_shaderHandlerObject.m_program->bind();
}

void FirstPass::initializations(std::vector<ModelLoader*>& models)
{

    m_shaderHandlerObject.compileShaders(":/shaders/FirstPass.vert",NULL,":/shaders/FirstPass.frag");
    m_shaderHandlerObject.m_program->bind();

    for(auto x: models)
    {
        QOpenGLVertexArrayObject *tempVAO = new QOpenGLVertexArrayObject();
        tempVAO->create();
        tempVAO->bind();

        //Don't forget to bind indices otherwise model will not draw.
        x->m_indicesID->bind();

        //Bind the required buffers and set the required information.
        x->m_vertexID.bind();
        m_shaderHandlerObject.m_program->enableAttributeArray(0);
        m_shaderHandlerObject.m_program->setAttributeBuffer(0,GL_FLOAT,0,3,0);
        x->m_normalID.bind();
        m_shaderHandlerObject.m_program->enableAttributeArray(1);
        m_shaderHandlerObject.m_program->setAttributeBuffer(1,GL_FLOAT,0,3,0);

        //releasing all information about the model.
        tempVAO->release();
        x->m_indicesID->release();
        x->m_vertexID.release();
        x->m_normalID.release();

        //push back to our local queue
        m_VAO.push_back(tempVAO);
    }

    //release the program
    m_shaderHandlerObject.m_program->release();
}

void FirstPass::setObjectData(int indexOfObjectCurrentlyDrawn)
{
    m_VAO[indexOfObjectCurrentlyDrawn]->bind();

}

void FirstPass::releaseProgramAndObjectData()
{
    for(auto temp:m_VAO)
    {
        temp->release();
    }
    m_shaderHandlerObject.m_program->release();

}
