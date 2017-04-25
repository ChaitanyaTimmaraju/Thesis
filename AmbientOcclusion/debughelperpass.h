#ifndef DEBUGHELPERPASS_H
#define DEBUGHELPERPASS_H

#include "passinterface.h"
#include "transform3d.h"

class DebugHelperPass : public PassInterface
{
public:
    DebugHelperPass(){}
    virtual void setUniforms();
    virtual void initializations(std::vector<ModelLoader*>&models);
    virtual void setObjectData(int);
    virtual void releaseProgramAndObjectData();
    Transform3D m_transform;
    QMatrix4x4 m_projection;
    int m_transformID,m_projectionID;
};

#endif // DEBUGHELPERPASS_H
