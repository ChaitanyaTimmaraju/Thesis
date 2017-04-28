#version 440
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 modelToWorld;
uniform mat4 worldToView;

out VS_OUT {
    vec3 eyePosition;
    vec3  normalPosition;
}vs_out;

void main()
{
  vec4 temp =modelToWorld*vec4(position, 1.0);
  gl_Position =worldToView*temp;
  vs_out.normalPosition = normalize((inverse(transpose(modelToWorld))*vec4(normal,0.0)).xyz);
  vs_out.eyePosition = temp.xyz;
}
