#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include<QString>
#include <QOpenGLShaderProgram>


class ShaderHandler
{

public:
    ShaderHandler(){}
    ShaderHandler(QString vertexShader,QString geometryShader,QString fragmentShader);
    void compileShaders(QString vertexShader,QString geometryShader,QString fragmentShader);
    ~ShaderHandler();
    QOpenGLShaderProgram *m_program;
};

#endif // SHADERHANDLER_H
