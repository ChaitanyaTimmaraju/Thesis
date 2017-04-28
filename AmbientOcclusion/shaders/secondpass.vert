#version 440
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 texCoords;


out vec2 f_UVS ;
uniform mat4 modelToWorld;
uniform mat4 worldToView;
out vec3 f_eyeSpacePositions;
out vec3 f_normal;
void main()
{
  vec4 eyeSpacePositions = modelToWorld*vec4(position, 1.0);
  gl_Position =worldToView*eyeSpacePositions;
  f_UVS = texCoords;
  f_eyeSpacePositions = eyeSpacePositions.xyz;
  f_normal =normalize((inverse(transpose(modelToWorld))*vec4(normal,0.0)).xyz);

}
