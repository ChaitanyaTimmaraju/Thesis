#ifndef PASSINTERFACE_H
#define PASSINTERFACE_H

#include <vector>
#include "shaderhandler.h"
#include "modelloader.h"

class QOpenGLVertexArrayObject;

/* This class is solely written for simplification
 * of the multipass rendering.
 */

class PassInterface
{
public:
    std::vector<QOpenGLVertexArrayObject*> m_VAO;
    ShaderHandler m_shaderHandlerObject;
public:
    virtual void setUniforms()=0;
    virtual void initializations(std::vector<ModelLoader*>&)=0;
    virtual void setObjectData(int)=0;
    virtual void releaseProgramAndObjectData()=0;
};

#endif // PASSINTERFACE_H
