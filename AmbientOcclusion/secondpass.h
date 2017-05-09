#ifndef SECONDPASS_H
#define SECONDPASS_H

#include "passinterface.h"
#include "transform3d.h"

class SecondPass : public PassInterface {
 public:
  SecondPass() {}
  virtual void setUniforms();
  virtual void initializations(std::vector<ModelLoader*>& models);
  virtual void setObjectData(int);
  virtual void releaseProgramAndObjectData();
  Transform3D m_transform;
  QMatrix4x4 m_projection;
  int m_transformID, m_projectionID;

 private:
  const int sampleSize = 25;
  const float sampleRadius = 0.1f;
};
#endif  // SECONDPASS_H
