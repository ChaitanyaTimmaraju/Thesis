#ifndef FIRSTPASS_H
#define FIRSTPASS_H

#include "passinterface.h"

class FirstPass : public PassInterface
{
public:
    FirstPass(){}
    virtual void setUniforms();
    virtual void initializations(std::vector<ModelLoader*>&models);
    virtual void setObjectData(int);
    virtual void releaseProgramAndObjectData();
};

#endif // FIRSTPASS_H
