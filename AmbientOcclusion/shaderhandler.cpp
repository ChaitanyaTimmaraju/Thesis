#include "shaderhandler.h"

ShaderHandler::ShaderHandler(QString vertexShader,QString geometryShader,QString fragmentShader)
{
   compileShaders(vertexShader,geometryShader,fragmentShader);
}
void ShaderHandler::compileShaders(QString vertexShader,QString geometryShader,QString fragmentShader)
{
    m_program =new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,vertexShader);
    if(!geometryShader.isNull())
        m_program->addShaderFromSourceFile(QOpenGLShader::Geometry,geometryShader);
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,fragmentShader);
    m_program->link();
    m_program->release();
}
ShaderHandler::~ShaderHandler()
{

    delete m_program;
}
