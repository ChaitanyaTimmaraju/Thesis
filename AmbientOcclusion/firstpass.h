#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include "passinterface.h"
#include "transform3d.h"

class FirstPass : public PassInterface
{
public:
    FirstPass(){}
    virtual void setUniforms();
    virtual void initializations(std::vector<ModelLoader*>&models);
    virtual void setObjectData(int);
    virtual void releaseProgramAndObjectData();
    Transform3D m_transform;
    int m_transformID,m_projectionID;
};

#endif // FIRSTPASS_H
