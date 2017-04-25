#version 440
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 texCoords;

uniform mat4 modelToWorld;
uniform mat4 worldToView;

out VS_OUT {
    vec3 eyePosition;
}vs_out;

void main()
{
  vec4 temp =modelToWorld*vec4(position, 1.0);
  gl_Position =worldToView*temp;

  vec4 lightDirection = normalize(vec4(0.0,0.0,-8.0,1.0)-temp);
  vec4 normalDirection = normalize(transpose(inverse(modelToWorld))*vec4(normal,0.0));

  vs_out.eyePosition = temp.xyz*0.5+0.5;
}
