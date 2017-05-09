#version 440 core
layout(location = 0) in vec3 position;

uniform mat4 modelToWorld;
uniform mat4 worldToView;
out vec3 f_eyeSpacePositions;

void main()
{
  vec4 eyeSpacePositions = modelToWorld*vec4(position, 1.0);
  gl_Position =worldToView*eyeSpacePositions;
  f_eyeSpacePositions = eyeSpacePositions.xyz;
}
